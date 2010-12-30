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

            // weld verts
            // TODO

            // Convert to engine-space (if necessary)
            string sceneSource = sceneElement.Element("Source").Value;
            if (sceneSource.StartsWith("3DS Max"))
            {
                foreach (KeyValuePair<int, RenderCommand> kvp in renderCommands_)
                    kvp.Value.Transform(CoordinateSystem.MaxToRornMatrix);
            }

            // write to disk
        }

        private void CompileMaterial(XElement materialElement)
        {
            // Parse common material data
            int id = int.Parse(materialElement.Element("Id").Value);
            Vector3 ambientColor = Vector3.Parse(materialElement.Element("AmbientColor").Value);
            Vector3 diffuseColor = Vector3.Parse(materialElement.Element("DiffuseColor").Value);
            Vector3 specularColor = Vector3.Parse(materialElement.Element("SpecularColor").Value);

            if (materialElement.Element("Maps") == null)
            {
                // no maps, so this must be untextured.
                var newRenderCommand = new UntexturedRenderCommand(ambientColor, diffuseColor, specularColor);
                renderCommands_.Add(id, newRenderCommand);
            }
        }

        private void CompileNode(XElement nodeElement)
        {
            // Parse node to model matrix
            var nodeToSceneMatrixElement = nodeElement.Element("NodeToSceneMatrix");
            Matrix3 nodeToSceneMatrix = Matrix3.Parse(nodeElement.Element("NodeToSceneMatrix").Value);

            var meshElement = nodeElement.Element("Mesh");
            if (meshElement != null)
            {
                CompileMesh(nodeToSceneMatrix, meshElement);
            }
        }

        private void CompileMesh(Matrix3 nodeToSceneMatrix, XElement meshElement)
        {
            foreach (var triangleElement in meshElement.Elements("Triangle"))
            {
                // Parse triangle data
                string[] triangleData = triangleElement.Value.Split(',');
                int v0 = int.Parse(triangleData[0]);
                int v1 = int.Parse(triangleData[1]);
                int v2 = int.Parse(triangleData[2]);
                int materialId = int.Parse(triangleData[3]);

                // Pull out the vertex elements we need using a LINQ query
                var v0Element = (from vertexElement in meshElement.Elements("Vertex")
                                 where int.Parse(vertexElement.Element("Index").Value) == v0
                                 select vertexElement).First();
                var v1Element = (from vertexElement in meshElement.Elements("Vertex")
                                 where int.Parse(vertexElement.Element("Index").Value) == v1
                                 select vertexElement).First();
                var v2Element = (from vertexElement in meshElement.Elements("Vertex")
                                 where int.Parse(vertexElement.Element("Index").Value) == v2
                                 select vertexElement).First();

                // Compile this triangle (using the appropriately typed render command)
                renderCommands_[materialId].ParseAndTransformTriangle(nodeToSceneMatrix, v0Element, v1Element, v2Element);
            }
        }

        private Dictionary<int, RenderCommand> renderCommands_;
    }
}
