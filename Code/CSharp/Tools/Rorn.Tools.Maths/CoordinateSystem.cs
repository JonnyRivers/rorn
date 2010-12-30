using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.Maths
{
    public static class CoordinateSystem
    {
        // Swap y and z axis, and negate z (rhs -> lhs)
        public static Matrix3 MaxToRornMatrix = new Matrix3(
            1, 0,  0,
            0, 0, -1,
            0, 1,  0,
            0, 0,  0);
    }
}
