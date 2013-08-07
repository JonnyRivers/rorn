using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.Maths
{
    public struct Vector2
    {
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public static Vector2 Parse(string text)
        {
            string[] vectorElements = text.Split(',');
            return new Vector2(
                float.Parse(vectorElements[0]),
                float.Parse(vectorElements[1]));
        }

        public void Save(System.IO.BinaryWriter binaryWriter)
        {
            binaryWriter.Write(X);
            binaryWriter.Write(Y);
        }

        public float X;
        public float Y;
    }
}
