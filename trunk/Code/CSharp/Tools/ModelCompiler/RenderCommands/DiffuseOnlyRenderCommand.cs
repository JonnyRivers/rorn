using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;

using Rorn.Tools.Maths;

namespace Rorn.Tools.ModelCompiler
{
    internal class DiffuseOnlyRenderCommand : RenderCommand
    {
        internal override void ParseAndTransformTriangle(Matrix3 nodeToModelMatrix, 
            XElement v0Element, XElement v1Element, XElement v2Element)
        {
            throw new NotImplementedException();
        }

        internal override void Save(System.IO.BinaryWriter binaryWriter)
        {
            throw new NotImplementedException();
        }

        internal override void Transform(Matrix3 transformMatrix)
        {
            throw new NotImplementedException();
        }
    }
}
