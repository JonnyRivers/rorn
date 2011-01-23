using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal class UntexturedVertex
    {
        internal UntexturedVertex(Vector4 position, Vector4 normal)
        {
            Position = position;
            Normal = normal;
        }

        internal void Save(System.IO.BinaryWriter binaryWriter)
        {
            Position.Save(binaryWriter);
            Normal.Save(binaryWriter);
        }

        internal void Transform(Matrix4x4 transformMatrix)
        {
            Position = transformMatrix * Position;
            Normal = transformMatrix * Normal;
        }

        internal static bool AreApproxEqual(UntexturedVertex lhs, UntexturedVertex rhs) 
        {
            return
                VertexComparer.PositionsAreApproxEqual(lhs.Position, rhs.Position) &&
                VertexComparer.NormalsAreApproxEqual(lhs.Normal, rhs.Normal);
        }

        internal Vector4 Position { get; private set; }
        internal Vector4 Normal { get; private set; }
    }
}
