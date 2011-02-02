#include "Matrix4x4.h"

#include <cmath>

using namespace Rorn::Maths;

Matrix4x4::Matrix4x4(void)
{
}


Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14,
					 float m21, float m22, float m23, float m24,
					 float m31, float m32, float m33, float m34,
					 float m41, float m42, float m43, float m44)
					 : M11(m11), M12(m12), M13(m13), M14(m14),
					   M21(m21), M22(m22), M23(m23), M24(m24),
					   M31(m31), M32(m32), M33(m33), M34(m34),
					   M41(m41), M42(m42), M43(m43), M44(m44)
{
}

Matrix4x4::Matrix4x4(const Vector3& row1, const Vector3& row2, const Vector3& row3, const Vector3& row4)
	: M11(row1.X), M12(row1.Y), M13(row1.Z), M14(0.0f),
	  M21(row2.X), M22(row2.Y), M23(row2.Z), M24(0.0f),
	  M31(row3.X), M32(row3.Y), M33(row3.Z), M34(0.0f),
	  M41(row4.X), M42(row4.Y), M43(row4.Z), M44(1.0f)
{
}

// In-place binary operations
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	*this = *this * rhs;

	return *this;
	
}

/*static*/ Matrix4x4 Matrix4x4::BuildYRotationMatrix(float angle)
{
	float sinAngle = sin(angle);
	float cosAngle = cos(angle);

	return Matrix4x4(
		cosAngle, 0.0f, -sinAngle, 0.0f,
		    0.0f, 1.0f,      0.0f, 0.0f,
		sinAngle, 0.0f,  cosAngle, 0.0f,
		    0.0f, 0.0f,      0.0f, 1.0f);
}

/*static*/ Matrix4x4 Matrix4x4::BuildIdentity()
{
	return Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f); 
}

/*static*/ Matrix4x4 Matrix4x4::Transpose(const Matrix4x4& source)
{
	return Matrix4x4(
		source.M11, source.M21, source.M31, source.M41,
		source.M12, source.M22, source.M32, source.M42,
		source.M13, source.M23, source.M33, source.M43,
		source.M14, source.M24, source.M34, source.M44);
}