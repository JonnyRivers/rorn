using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.Maths
{
    public struct Vector3
    {
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 Parse(string text)
        {
            string[] vectorElements = text.Split(',');
            return new Vector3(
                float.Parse(vectorElements[0]),
                float.Parse(vectorElements[1]),
                float.Parse(vectorElements[2]));
        }

        public float X;
        public float Y;
        public float Z;
    }
}
