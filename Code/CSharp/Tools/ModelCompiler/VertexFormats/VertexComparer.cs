using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.ModelCompiler
{
    internal class VertexComparer
    {
        private static float positionElementTolerance = 0.000001f;
        private static float normalElementTolerance = 0.000001f;
        private static float uvElementTolerance = 0.000001f;

        internal static bool PositionsAreApproxEqual(Maths.Vector4 lhs, Maths.Vector4 rhs)
        {
            return
                FloatsAreApproxEqual(lhs.X, rhs.X, positionElementTolerance) &&
                FloatsAreApproxEqual(lhs.Y, rhs.Y, positionElementTolerance) &&
                FloatsAreApproxEqual(lhs.Z, rhs.Z, positionElementTolerance);
        }

        internal static bool NormalsAreApproxEqual(Maths.Vector4 lhs, Maths.Vector4 rhs)
        {
            return
                FloatsAreApproxEqual(lhs.X, rhs.X, normalElementTolerance) &&
                FloatsAreApproxEqual(lhs.Y, rhs.Y, normalElementTolerance) &&
                FloatsAreApproxEqual(lhs.Z, rhs.Z, normalElementTolerance);
        }

        internal static bool UVsAreApproxEqual(Maths.Vector2 lhs, Maths.Vector2 rhs)
        {
            return
                FloatsAreApproxEqual(lhs.X, rhs.X, uvElementTolerance) &&
                FloatsAreApproxEqual(lhs.Y, rhs.Y, uvElementTolerance);
        }

        private static bool FloatsAreApproxEqual(float lhs, float rhs, float tolerance)
        {
            return
                rhs >= (lhs - tolerance) &&
                rhs <= (lhs + tolerance);
        }
    }
}
