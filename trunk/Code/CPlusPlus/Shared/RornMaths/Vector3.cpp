#include "Vector3.h"

#include <cmath>

using namespace Rorn::Maths;

// Construction
Vector3::Vector3(void)
{
}

Vector3::Vector3(float x, float y, float z) : X(x), Y(y), Z(z)
{
}

Vector3::Vector3(const Vector3& source) : X(source.X), Y(source.Y), Z(source.Z)
{

}

// Assignemnt
Vector3& Vector3::operator=(const Vector3& source)
{
	X = source.X;
	Y = source.Y;
	Z = source.Z;

	return *this;
}

// In-place binary operations
Vector3& Vector3::operator+=(const Vector3& rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	Z += rhs.Z;

	return *this;
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	Z -= rhs.Z;

	return *this;
}

Vector3& Vector3::operator*=(float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;

	return *this;
}

Vector3& Vector3::operator/=(float rhs)
{
	float rhsReciprocal = 1.0f / rhs;

	X *= rhsReciprocal;
	Y *= rhsReciprocal;
	Z *= rhsReciprocal;

	return *this;
}

// Member operations
float Vector3::GetLength() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

// Static member operations
/*static*/ Vector3 Vector3::CrossProduct(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3(
		lhs.Y * rhs.Z - lhs.Z * rhs.Y,
		lhs.Z * rhs.X - lhs.X * rhs.Z,
		lhs.X * rhs.Y - lhs.Y * rhs.X);
}

/*static*/ float Vector3::DotProduct(const Vector3& lhs, const Vector3& rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

/*static*/ Vector3 Vector3::GetUnitVector(const Vector3& source)
{
	float length = source.GetLength();
	return source / length;
}

