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
            foreach (var materialElement in sceneElement.Elements("Material"))
            {
                CompileMaterial(materialElement);
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

        private void CompileMaterial(XElement materialElement)
        {
            // Parse common material data
            int id = int.Parse(materialElement.Element("Id").Value);
            Vector4 ambientColor = Vector4.Parse(materialElement.Element("AmbientColor").Value, 1.0f);
            Vector4 diffuseColor = Vector4.Parse(materialElement.Element("DiffuseColor").Value, 1.0f);
            Vector4 specularColor = Vector4.Parse(materialElement.Element("SpecularColor").Value, 1.0f);
            float phongExponent = float.Parse(materialElement.Element("PhongExponent").Value);

            if (materialElement.Element("Maps") == null)
            {
                // no maps, so this must be untextured.
                var newRenderCommand = new UntexturedRenderCommand(ambientColor, diffuseColor, specularColor, phongExponent);
                renderCommands_.Add(id, newRenderCommand);
            }
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
            using (var binaryWriter = new System.IO.BinaryWriter(System.IO.File.Create(destinationPathName)))
            {
                binaryWriter.Write(renderCommands_.Count);
                foreach (KeyValuePair<int, RenderCommand> kvp in renderCommands_)
                {
                    kvp.Value.Save(binaryWriter);
                }
            }
        }

        private Dictionary<int, RenderCommand> renderCommands_;
    }
}
