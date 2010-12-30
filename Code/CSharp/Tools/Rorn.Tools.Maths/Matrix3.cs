using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.Maths
{
    public struct Matrix3
    {
        public Matrix3(
            float m11, float m12, float m13, 
            float m21, float m22, float m23, 
            float m31, float m32, float m33, 
            float tx, float ty, float tz)
        {
            M11 = m11;
            M12 = m12;
            M13 = m13;
            M21 = m21;
            M22 = m22;
            M23 = m23;
            M31 = m31;
            M32 = m32;
            M33 = m33;
            TX = tx;
            TY = ty;
            TZ = tz;
        }

        public Matrix3 WithoutTranslation()
        {
            return new Matrix3(
                M11, M12, M13,
                M21, M22, M23,
                M31, M32, M33,
                0,     0,   0);

        }

        public static Matrix3 Parse(string text)
        {
            string[] matrixElements = text.Split(',');
            return new Matrix3(
                float.Parse(matrixElements[0]),
                float.Parse(matrixElements[1]),
                float.Parse(matrixElements[2]),
                float.Parse(matrixElements[3]),
                float.Parse(matrixElements[4]),
                float.Parse(matrixElements[5]),
                float.Parse(matrixElements[6]),
                float.Parse(matrixElements[7]),
                float.Parse(matrixElements[8]),
                float.Parse(matrixElements[9]),
                float.Parse(matrixElements[10]),
                float.Parse(matrixElements[11]));
        }

        public static Vector3 operator*(Matrix3 matrix, Vector3 vector)
        {
            return new Vector3(
                vector.X * matrix.M11 + vector.Y * matrix.M21 + vector.Z * matrix.M31 + matrix.TX,
                vector.X * matrix.M12 + vector.Y * matrix.M22 + vector.Z * matrix.M32 + matrix.TY,
                vector.X * matrix.M13 + vector.Y * matrix.M23 + vector.Z * matrix.M33 + matrix.TZ);

        }

        public float M11;
        public float M12;
        public float M13;
        public float M21;
        public float M22;
        public float M23;
        public float M31;
        public float M32;
        public float M33;
        public float TX;
        public float TY;
        public float TZ;
    }
}
