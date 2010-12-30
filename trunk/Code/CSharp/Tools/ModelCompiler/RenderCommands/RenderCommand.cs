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
        internal abstract void ParseAndTransformTriangle(Matrix3 nodeToModelMatrix, 
            XElement v0Element, XElement v1Element, XElement v2Element);

        internal abstract void Transform(Matrix3 transformMatrix);
    }
}
