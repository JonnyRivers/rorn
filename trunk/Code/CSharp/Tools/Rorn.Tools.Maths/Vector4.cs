using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.Maths
{
    public struct Vector4
    {
        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public static Vector4 Parse(string text)
        {
            string[] vectorElements = text.Split(',');
            return new Vector4(
                float.Parse(vectorElements[0]),
                float.Parse(vectorElements[1]),
                float.Parse(vectorElements[2]),
                float.Parse(vectorElements[3]));
        }

        public static Vector4 Parse(string text, float w)
        {
            string[] vectorElements = text.Split(',');
            return new Vector4(
                float.Parse(vectorElements[0]),
                float.Parse(vectorElements[1]),
                float.Parse(vectorElements[2]),
                w);
        }

        public void Save(System.IO.BinaryWriter binaryWriter)
        {
            binaryWriter.Write(X);
            binaryWriter.Write(Y);
            binaryWriter.Write(Z);
            binaryWriter.Write(W);
        }

        public float X;
        public float Y;
        public float Z;
        public float W;
    }
}
