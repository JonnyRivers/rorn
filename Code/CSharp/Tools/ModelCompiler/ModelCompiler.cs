using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal class ModelCompiler
    {
        internal void Compile(string sourcePathName, string destinationPathName)
        {
            var sceneDocument = XDocument.Load(sourcePathName);
            var sceneElement = sceneDocument.Root;

            // for each material, parse the details and make a render command of the appropriate type
            renderCommands_ = new Dictionary<int, RenderCommand>();
            compiledTextures_ = new Dictionary<string, CompiledTexture>();

            string texturesDirectory = System.IO.Path.GetDirectoryName(sourcePathName);
            foreach (var materialElement in sceneElement.Elements("Material"))
            {
                CompileMaterial(materialElement, texturesDirectory);
            }

            // for each mesh, parse the triangles, populating the vert and index list of the appropriate render command as we go
            foreach (var nodeElement in sceneElement.Elements("Node"))
            {
                CompileNode(nodeElement);
            }

            // optimize mesh
            foreach (KeyValuePair<int, RenderCommand> kvp in renderCommands_)
                kvp.Value.Optimize();

            // Convert to engine-space (if necessary)
            string sceneSource = sceneElement.Element("Source").Value;
            if (sceneSource.StartsWith("3DS Max"))
            {
                foreach (KeyValuePair<int, RenderCommand> kvp in renderCommands_)
                    kvp.Value.Transform(CoordinateSystem.MaxToRornMatrix);
            }

            // write to disk
            SaveModel(destinationPathName);

            // flush data
            renderCommands_.Clear();
        }

        private void CompileMaterial(XElement materialElement, string texturesDirectory)
        {
            // Parse common material data
            int id = int.Parse(materialElement.Element("Id").Value);
            Vector4 ambientColor = Vector4.Parse(materialElement.Element("AmbientColor").Value, 1.0f);
            Vector4 diffuseColor = Vector4.Parse(materialElement.Element("DiffuseColor").Value, 1.0f);
            Vector4 specularColor = Vector4.Parse(materialElement.Element("SpecularColor").Value, 1.0f);
            float phongExponent = float.Parse(materialElement.Element("PhongExponent").Value);

            XElement textureMapsElement = materialElement.Element("TextureMaps");
            if (textureMapsElement == null)
            {
                // no maps, so this must be untextured.
                var newRenderCommand = new UntexturedRenderCommand(ambientColor, diffuseColor, specularColor, phongExponent);
                renderCommands_.Add(id, newRenderCommand);
            }
            else
            {
                // Diffuse only material
                XElement diffuseMapElement = textureMapsElement.Element("DiffuseMap");
                string diffuseMapFileName = diffuseMapElement.Element("PathName").Value;
                int diffuseMapId = CompileTexture(
                    string.Format("{0}\\{1}", texturesDirectory, diffuseMapFileName));
                var newRenderCommand = new DiffuseOnlyRenderCommand(ambientColor, diffuseColor, specularColor, phongExponent, diffuseMapId);
                renderCommands_.Add(id, newRenderCommand);
            }
        }

        private int CompileTexture(string texturePathName)
        {
            // TODO - should replace this with a case insensitive comparisson
            string texturePathNameUpper = texturePathName.ToUpper();
            if(compiledTextures_.ContainsKey(texturePathNameUpper))
                return compiledTextures_[texturePathNameUpper].Id;

            int compiledTextureId = compiledTextures_.Count;

            // Run nvcompress, which must live alongside the executing assembly (for now)
            string modelCompilerPathName = System.Reflection.Assembly.GetExecutingAssembly().Location;
            string modelCompilerDirectory = System.IO.Path.GetDirectoryName(modelCompilerPathName);
            string nvcompressPathName = string.Format("{0}\\nvcompress.exe", modelCompilerDirectory);
            string tempTargetPathName = System.IO.Path.GetTempFileName();
            string arguments = string.Format("-alpha -fast -bc3 \"{0}\" \"{1}\"", texturePathName, tempTargetPathName);
            var processStartInfo = new System.Diagnostics.ProcessStartInfo(nvcompressPathName, arguments);
            processStartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
            var nvcompressProcess = System.Diagnostics.Process.Start(processStartInfo);
            nvcompressProcess.WaitForExit();
            byte[] compressedTextureData = System.IO.File.ReadAllBytes(tempTargetPathName);
            System.IO.File.Delete(tempTargetPathName);
            compiledTextures_.Add(texturePathNameUpper, new CompiledTexture(compiledTextureId, compressedTextureData));

            return compiledTextureId;
        }

        private void CompileNode(XElement nodeElement)
        {
            // Parse node to model matrix
            var nodeToSceneMatrixElement = nodeElement.Element("NodeToSceneMatrix");
            Matrix4x4 nodeToSceneMatrix = Matrix4x4.Parse(nodeElement.Element("NodeToSceneMatrix").Value);

            var meshElement = nodeElement.Element("Mesh");
            if (meshElement != null)
            {
                CompileMesh(nodeToSceneMatrix, meshElement);
            }
        }

        private void CompileMesh(Matrix4x4 nodeToSceneMatrix, XElement meshElement)
        {
            // Break the vertex elements out into an indexed list, otherwise we'll be here all day!
            Dictionary<int, XElement> indexedVertexElements = new Dictionary<int, XElement>();
            foreach (var vertexElement in meshElement.Elements("Vertex"))
            {
                indexedVertexElements.Add(
                    int.Parse(vertexElement.Element("Index").Value),
                    vertexElement);
            }

            foreach (var triangleElement in meshElement.Elements("Triangle"))
            {
                // Parse triangle data
                string[] triangleData = triangleElement.Value.Split(',');
                int vert0Index = int.Parse(triangleData[0]);
                int vert1Index = int.Parse(triangleData[1]);
                int vert2Index = int.Parse(triangleData[2]);
                int materialId = int.Parse(triangleData[3]);

                // Compile this triangle (using the appropriately typed render command)
                renderCommands_[materialId].ParseAndTransformTriangle(
                    nodeToSceneMatrix,
                    indexedVertexElements[vert0Index],
                    indexedVertexElements[vert1Index],
                    indexedVertexElements[vert2Index]);
            }
        }

        private void SaveModel(string destinationPathName)
        {
            // Generate header information
            BoundingBox boundingBox = new BoundingBox();
            foreach (KeyValuePair<int, RenderCommand> kvp in renderCommands_)
                kvp.Value.IncorporatePointIntoBoundingBox(boundingBox);
            ModelHeader modelHeader = new ModelHeader(boundingBox, renderCommands_.Count, compiledTextures_.Count);

            using (var binaryWriter = new System.IO.BinaryWriter(System.IO.File.Create(destinationPathName)))
            {
                modelHeader.Save(binaryWriter);

                foreach (KeyValuePair<string, CompiledTexture> kvp in compiledTextures_)
                    kvp.Value.Save(binaryWriter);

                foreach (KeyValuePair<int, RenderCommand> kvp in renderCommands_)
                    kvp.Value.Save(binaryWriter);
            }
        }

        private Dictionary<int, RenderCommand> renderCommands_;
        private Dictionary<string, CompiledTexture> compiledTextures_;
    }
}
