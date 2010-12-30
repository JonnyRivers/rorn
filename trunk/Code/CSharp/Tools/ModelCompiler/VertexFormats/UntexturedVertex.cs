using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal class UntexturedVertex
    {
        internal UntexturedVertex(Vector3 position, Vector3 normal)
        {
            position_ = position;
            normal_ = normal;
        }

        internal void Transform(Matrix3 transformMatrix)
        {
            position_ = transformMatrix * position_;
            normal_ = transformMatrix.WithoutTranslation() * normal_;
        }

        private Vector3 position_;
        private Vector3 normal_;
    }
}
