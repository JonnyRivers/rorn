using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal class DiffuseOnlyVertex
    {
        internal DiffuseOnlyVertex(Vector4 position, Vector4 normal, Vector2 diffuseUV)
        {
            Position = position;
            Normal = normal;
            DiffuseUV = diffuseUV;
        }

        internal void Save(System.IO.BinaryWriter binaryWriter)
        {
            Position.Save(binaryWriter);
            Normal.Save(binaryWriter);
            DiffuseUV.Save(binaryWriter);
        }

        internal void Transform(Matrix4x4 transformMatrix)
        {
            Position = transformMatrix * Position;
            Normal = transformMatrix * Normal;
        }

        internal static bool AreApproxEqual(DiffuseOnlyVertex lhs, DiffuseOnlyVertex rhs) 
        {
            return
                VertexComparer.PositionsAreApproxEqual(lhs.Position, rhs.Position) &&
                VertexComparer.NormalsAreApproxEqual(lhs.Normal, rhs.Normal) &&
                VertexComparer.UVsAreApproxEqual(lhs.DiffuseUV, rhs.DiffuseUV);
        }

        internal Vector4 Position { get; private set; }
        internal Vector4 Normal { get; private set; }
        internal Vector2 DiffuseUV { get; private set; }
    }
}
