﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Rorn.Tools.Maths
{
    public struct Matrix4x4
    {
        public Matrix4x4(
            float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44)
        {
            M11 = m11;
            M12 = m12;
            M13 = m13;
            M14 = m14;
            M21 = m21;
            M22 = m22;
            M23 = m23;
            M24 = m24;
            M31 = m31;
            M32 = m32;
            M33 = m33;
            M34 = m34;
            M41 = m41;
            M42 = m42;
            M43 = m43;
            M44 = m44;
        }

        public static Matrix4x4 Parse(string text)
        {
            string[] matrixElements = text.Split(',');
            if (matrixElements.Count() == 12)
            {
                return new Matrix4x4(
                    float.Parse(matrixElements[0]),
                    float.Parse(matrixElements[1]),
                    float.Parse(matrixElements[2]),
                    0.0f,
                    float.Parse(matrixElements[3]),
                    float.Parse(matrixElements[4]),
                    float.Parse(matrixElements[5]),
                    0.0f,
                    float.Parse(matrixElements[6]),
                    float.Parse(matrixElements[7]),
                    float.Parse(matrixElements[8]),
                    0.0f,
                    float.Parse(matrixElements[9]),
                    float.Parse(matrixElements[10]),
                    float.Parse(matrixElements[11]),
                    1.0f);
            }
            else if (matrixElements.Count() == 16)
            {
                return new Matrix4x4(
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
                    float.Parse(matrixElements[11]),
                    float.Parse(matrixElements[12]),
                    float.Parse(matrixElements[13]),
                    float.Parse(matrixElements[14]),
                    float.Parse(matrixElements[15]));
            }

            throw new FormatException("text must contain 12 or 16 floats");
        }

        public static Vector4 operator*(Matrix4x4 matrix, Vector4 vector)
        {
            return new Vector4(
                vector.X * matrix.M11 + vector.Y * matrix.M21 + vector.Z * matrix.M31 + vector.W * matrix.M41,
                vector.X * matrix.M12 + vector.Y * matrix.M22 + vector.Z * matrix.M32 + vector.W * matrix.M42,
                vector.X * matrix.M13 + vector.Y * matrix.M23 + vector.Z * matrix.M33 + vector.W * matrix.M43,
                vector.X * matrix.M14 + vector.Y * matrix.M24 + vector.Z * matrix.M34 + vector.W * matrix.M44);
        }

        public float M11;
        public float M12;
        public float M13;
        public float M14;
        public float M21;
        public float M22;
        public float M23;
        public float M24;
        public float M31;
        public float M32;
        public float M33;
        public float M34;
        public float M41;
        public float M42;
        public float M43;
        public float M44;
    }
}