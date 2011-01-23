using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal abstract class RenderCommand
    {
        internal enum Type
        {
            Untextured = 0,
            DiffuseOnly = 1
        }

        internal enum PrimitiveType
        {
            TriangleList = 0
        }

        internal abstract void Optimize();

        internal abstract void ParseAndTransformTriangle(Matrix4x4 nodeToModelMatrix, 
            XElement v0Element, XElement v1Element, XElement v2Element);

        internal abstract void Save(System.IO.BinaryWriter binaryWriter);

        internal abstract void Transform(Matrix4x4 transformMatrix);
    }
}
