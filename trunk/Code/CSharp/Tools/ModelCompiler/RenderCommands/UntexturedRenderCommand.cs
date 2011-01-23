using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal class UntexturedRenderCommand : RenderCommand
    {
        internal UntexturedRenderCommand(Vector4 ambientColor, Vector4 diffuseColor, Vector4 specularColor, float phongExponent)
        {
            ambientColor_ = ambientColor;
            diffuseColor_ = diffuseColor;
            specularColor_ = specularColor;
            phongExponent_ = phongExponent;
            vertices_ = new List<UntexturedVertex>();
            indices_ = new List<int>();
        }

        internal override void IncorporatePointIntoBoundingBox(BoundingBox boundingBox)
        {
            foreach (UntexturedVertex vertex in vertices_)
            {
                boundingBox.AddPoint(vertex.Position);
            }
        }

        internal override void Optimize()
        {
            // build a new vertex buffer and a new index buffer
            var optimizedVertices = new List<UntexturedVertex>();
            var optimizedIndices = new List<int>();

            foreach (int index in indices_) 
            {
                UntexturedVertex thisVertex = vertices_[index];
                int thisVertexIndex = 0;

                bool foundMatch = false;
                foreach (UntexturedVertex optimizedVertex in optimizedVertices)
                {
                    if ((thisVertex != optimizedVertex) && (UntexturedVertex.AreApproxEqual(thisVertex, optimizedVertex)))
                    {
                        optimizedIndices.Add(thisVertexIndex);
                        foundMatch = true;
                        break;
                    }

                    ++thisVertexIndex;
                }

                if (!foundMatch)
                {
                    // Add a new vertex to the optimized list
                    optimizedIndices.Add(optimizedVertices.Count);
                    optimizedVertices.Add(thisVertex);
                }
            }

            vertices_ = optimizedVertices;
            indices_ = optimizedIndices;
        }

        internal override void ParseAndTransformTriangle(Matrix4x4 nodeToModelMatrix, 
            XElement v0Element, XElement v1Element, XElement v2Element)
        {
            UntexturedVertex v0 = ParseAndTransformVertex(nodeToModelMatrix, v0Element);
            UntexturedVertex v1 = ParseAndTransformVertex(nodeToModelMatrix, v1Element);
            UntexturedVertex v2 = ParseAndTransformVertex(nodeToModelMatrix, v2Element);

            vertices_.Add(v0);
            indices_.Add(indices_.Count);
            vertices_.Add(v1);
            indices_.Add(indices_.Count);
            vertices_.Add(v2);
            indices_.Add(indices_.Count);
        }

        internal override void Save(System.IO.BinaryWriter binaryWriter)
        {
            binaryWriter.Write((int)RenderCommand.Type.Untextured);
            binaryWriter.Write((int)RenderCommand.PrimitiveType.TriangleList);

            ambientColor_.Save(binaryWriter);
            diffuseColor_.Save(binaryWriter);
            specularColor_.Save(binaryWriter);
            binaryWriter.Write(phongExponent_);

            binaryWriter.Write(vertices_.Count);
            foreach (UntexturedVertex vertex in vertices_)
                vertex.Save(binaryWriter);

            binaryWriter.Write(indices_.Count);
            foreach (int index in indices_)
                binaryWriter.Write(index);
        }

        internal override void Transform(Matrix4x4 transformMatrix)
        {
            foreach (UntexturedVertex vertex in vertices_)
            {
                vertex.Transform(transformMatrix);
            }
        }

        private UntexturedVertex ParseAndTransformVertex(Matrix4x4 nodeToModelMatrix, XElement vertexElement)
        {
            Vector4 position = Vector4.Parse(vertexElement.Element("Position").Value, 1.0f);
            position = nodeToModelMatrix * position;
            Vector4 normal = Vector4.Parse(vertexElement.Element("Normal").Value, 0.0f);
            normal = nodeToModelMatrix * normal;

            return new UntexturedVertex(position, normal);
        }

        private Vector4 ambientColor_;
        private Vector4 diffuseColor_;
        private Vector4 specularColor_;
        private float phongExponent_;
        private List<UntexturedVertex> vertices_;
        private List<int> indices_;
    }
}
