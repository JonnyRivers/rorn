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
            position_ = position;
            normal_ = normal;
        }

        internal void Save(System.IO.BinaryWriter binaryWriter)
        {
            position_.Save(binaryWriter);
            normal_.Save(binaryWriter);
        }

        internal void Transform(Matrix4x4 transformMatrix)
        {
            position_ = transformMatrix * position_;
            normal_ = transformMatrix * normal_;
        }

        internal static bool AreApproxEqual(UntexturedVertex lhs, UntexturedVertex rhs) 
        {
            return
                VertexComparer.PositionsAreApproxEqual(lhs.position_, rhs.position_) &&
                VertexComparer.NormalsAreApproxEqual(lhs.normal_, rhs.normal_);
        }

        private Vector4 position_;
        private Vector4 normal_;
    }
}
