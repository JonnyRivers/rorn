﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal class UntexturedRenderCommand : RenderCommand
    {
        internal UntexturedRenderCommand(Vector3 ambientColor, Vector3 diffuseColor, Vector3 specularColor, float phongExponent)
        {
            ambientColor_ = ambientColor;
            diffuseColor_ = diffuseColor;
            specularColor_ = specularColor;
            phongExponent_ = phongExponent;
            vertices_ = new List<UntexturedVertex>();
            indices_ = new List<int>();
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

        internal override void ParseAndTransformTriangle(Matrix4x3 nodeToModelMatrix, 
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

        internal override void Transform(Matrix4x3 transformMatrix)
        {
            foreach (UntexturedVertex vertex in vertices_)
            {
                vertex.Transform(transformMatrix);
            }
        }

        private UntexturedVertex ParseAndTransformVertex(Matrix4x3 nodeToModelMatrix, XElement vertexElement)
        {
            Vector3 position = Vector3.Parse(vertexElement.Element("Position").Value);
            position = nodeToModelMatrix * position;
            Vector3 normal = Vector3.Parse(vertexElement.Element("Normal").Value);
            normal = nodeToModelMatrix.WithoutTranslation() * normal;

            return new UntexturedVertex(position, normal);
        }

        private Vector3 ambientColor_;
        private Vector3 diffuseColor_;
        private Vector3 specularColor_;
        private float phongExponent_;
        private List<UntexturedVertex> vertices_;
        private List<int> indices_;
    }
}