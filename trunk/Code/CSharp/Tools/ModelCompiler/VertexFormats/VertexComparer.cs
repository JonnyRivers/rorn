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

        internal static bool PositionsAreApproxEqual(Maths.Vector3 lhs, Maths.Vector3 rhs)
        {
            return
                FloatsAreApproxEqual(lhs.X, rhs.X, positionElementTolerance) &&
                FloatsAreApproxEqual(lhs.Y, rhs.Y, positionElementTolerance) &&
                FloatsAreApproxEqual(lhs.Z, rhs.Z, positionElementTolerance);
        }

        internal static bool NormalsAreApproxEqual(Maths.Vector3 lhs, Maths.Vector3 rhs)
        {
            return
                FloatsAreApproxEqual(lhs.X, rhs.X, normalElementTolerance) &&
                FloatsAreApproxEqual(lhs.Y, rhs.Y, normalElementTolerance) &&
                FloatsAreApproxEqual(lhs.Z, rhs.Z, normalElementTolerance);
        }

        private static bool FloatsAreApproxEqual(float lhs, float rhs, float tolerance)
        {
            return
                rhs >= (lhs - tolerance) &&
                rhs <= (lhs + tolerance);
        }
    }
}
