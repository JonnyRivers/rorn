#include "Matrix4x4.h"

#include <cassert>
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

// In-place binary operations
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs)
{
	*this = *this * rhs;

	return *this;
	
}

/*static*/  Matrix4x4 Matrix4x4::BuildRotationMatrix(const UnitDirection& axis, float angle)
{
	assert(axis.GetLength() - 1.0f < 0.001f);// check the axis is a unit vector

	float sinAngle = sin(angle);
	float cosAngle = cos(angle);

	// Compute some common subexpressions
	float a = 1.0f - cosAngle;
	float ax = a * axis.X;
	float ay = a * axis.Y;
	float az = a * axis.Z;

	return Matrix4x4(
		ax * axis.X + cosAngle, 
		ax * axis.Y + axis.Z * sinAngle, 
		ax * axis.Z - axis.Y * sinAngle,
		0.0f,
		ay * axis.X - axis.Z * sinAngle, 
		ay * axis.Y + cosAngle, 
		ay * axis.Z + axis.X * sinAngle,
		0.0f,
		az * axis.X + axis.Y * sinAngle, 
		az * axis.Y - axis.X * sinAngle, 
		az * axis.Z + cosAngle,
		0.0f,
		0.0f, 
		0.0f, 
		0.0f, 
		1.0f);
}

/*static*/ Matrix4x4 Matrix4x4::BuildXRotationMatrix(float angle)
{
	float sinAngle = sin(angle);
	float cosAngle = cos(angle);

	return Matrix4x4(
		1.0f,  0.0f,     0.0f,     0.0f,
		0.0f,  cosAngle, sinAngle, 0.0f,
		0.0f, -sinAngle, cosAngle, 0.0f,
		0.0f,  0.0f,     0.0f,     1.0f);
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

/*static*/ Matrix4x4 Matrix4x4::BuildZRotationMatrix(float angle)
{
	float sinAngle = sin(angle);
	float cosAngle = cos(angle);

	return Matrix4x4(
		 cosAngle, sinAngle, 0.0f, 0.0f,
		-sinAngle, cosAngle, 0.0f, 0.0f,
		     0.0f, 0.0f,     1.0f, 0.0f,
		     0.0f, 0.0f,     0.0f, 1.0f);
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