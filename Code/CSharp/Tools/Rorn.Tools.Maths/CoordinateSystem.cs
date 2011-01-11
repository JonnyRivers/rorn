﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.Maths
{
    public static class CoordinateSystem
    {
        // Swap y and z axis, and negate z (rhs -> lhs)
        public static Matrix4x3 MaxToRornMatrix = new Matrix4x3(
            1, 0,  0,
            0, 0, -1,
            0, 1,  0,
            0, 0,  0);
    }
}
