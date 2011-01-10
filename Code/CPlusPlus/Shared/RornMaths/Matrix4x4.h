#pragma once

#include "Vector3.h"

namespace Rorn
{
	namespace Maths
	{
		class Matrix4x4
		{
		public:
			// Data
			float M11, M12, M13, M14;
			float M21, M22, M23, M24;
			float M31, M32, M33, M34;
			float M41, M42, M43, M44;

			// Construction
			Matrix4x4(void);
			Matrix4x4(float m11, float m12, float m13, float m14,
					  float m21, float m22, float m23, float m24,
					  float m31, float m32, float m33, float m34,
					  float m41, float m42, float m43, float m44);
			Matrix4x4(const Vector3& row1, const Vector3& row2, const Vector3& row3, const Vector3& row4);

			// In-place binary operations
			Matrix4x4& operator*=(const Matrix4x4& rhs);

			// Static member operations
			static Matrix4x4 BuildYRotationMatrix(float angle);
			static Matrix4x4 BuildIdentity();
			static Matrix4x4 Transpose(const Matrix4x4& source);
		};

		// Binary non-member operations
		static Matrix4x4 operator*(const Matrix4x4& lhs, const Matrix4x4& rhs)
		{
			return Matrix4x4(
				(lhs.M11 * rhs.M11) + (lhs.M12 * rhs.M21) + (lhs.M13 * rhs.M31) + (lhs.M14 * rhs.M41),
				(lhs.M11 * rhs.M12) + (lhs.M12 * rhs.M22) + (lhs.M13 * rhs.M32) + (lhs.M14 * rhs.M42),
				(lhs.M11 * rhs.M13) + (lhs.M12 * rhs.M23) + (lhs.M13 * rhs.M33) + (lhs.M14 * rhs.M43),
				(lhs.M11 * rhs.M14) + (lhs.M12 * rhs.M24) + (lhs.M13 * rhs.M34) + (lhs.M14 * rhs.M44),

				(lhs.M21 * rhs.M11) + (lhs.M22 * rhs.M21) + (lhs.M23 * rhs.M31) + (lhs.M24 * rhs.M41),
				(lhs.M21 * rhs.M12) + (lhs.M22 * rhs.M22) + (lhs.M23 * rhs.M32) + (lhs.M24 * rhs.M42),
				(lhs.M21 * rhs.M13) + (lhs.M22 * rhs.M23) + (lhs.M23 * rhs.M33) + (lhs.M24 * rhs.M43),
				(lhs.M21 * rhs.M14) + (lhs.M22 * rhs.M24) + (lhs.M23 * rhs.M34) + (lhs.M24 * rhs.M44),

				(lhs.M31 * rhs.M11) + (lhs.M32 * rhs.M21) + (lhs.M33 * rhs.M31) + (lhs.M34 * rhs.M41),
				(lhs.M31 * rhs.M12) + (lhs.M32 * rhs.M22) + (lhs.M33 * rhs.M32) + (lhs.M34 * rhs.M42),
				(lhs.M31 * rhs.M13) + (lhs.M32 * rhs.M23) + (lhs.M33 * rhs.M33) + (lhs.M34 * rhs.M43),
				(lhs.M31 * rhs.M14) + (lhs.M32 * rhs.M24) + (lhs.M33 * rhs.M34) + (lhs.M34 * rhs.M44),

				(lhs.M41 * rhs.M11) + (lhs.M42 * rhs.M21) + (lhs.M43 * rhs.M31) + (lhs.M44 * rhs.M41),
				(lhs.M41 * rhs.M12) + (lhs.M42 * rhs.M22) + (lhs.M43 * rhs.M32) + (lhs.M44 * rhs.M42),
				(lhs.M41 * rhs.M13) + (lhs.M42 * rhs.M23) + (lhs.M43 * rhs.M33) + (lhs.M44 * rhs.M43),
				(lhs.M41 * rhs.M14) + (lhs.M42 * rhs.M24) + (lhs.M43 * rhs.M34) + (lhs.M44 * rhs.M44));
		}
	}
}