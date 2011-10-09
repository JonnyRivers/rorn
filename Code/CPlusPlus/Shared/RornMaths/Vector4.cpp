#include "Vector4.h"

#include <cmath>

using namespace Rorn::Maths;

// Construction
Vector4::Vector4(void)
{
}

Vector4::Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w)
{
}

Vector4::Vector4(const Vector4& source) : X(source.X), Y(source.Y), Z(source.Z), W(source.W)
{

}

// Assignemnt
Vector4& Vector4::operator=(const Vector4& source)
{
	X = source.X;
	Y = source.Y;
	Z = source.Z;
	W = source.W;

	return *this;
}

// In-place binary operations
Vector4& Vector4::operator+=(const Vector4& rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	Z += rhs.Z;
	W += rhs.W;

	return *this;
}

Vector4& Vector4::operator-=(const Vector4& rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	Z -= rhs.Z;
	W -= rhs.W;

	return *this;
}

Vector4& Vector4::operator*=(float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;

	return *this;
}

Vector4& Vector4::operator/=(float rhs)
{
	float rhsReciprocal = 1.0f / rhs;

	X *= rhsReciprocal;
	Y *= rhsReciprocal;
	Z *= rhsReciprocal;
	W *= rhsReciprocal;

	return *this;
}

// Member operations
float Vector4::GetLength() const
{
	return sqrt(X * X + Y * Y + Z * Z + W * W);
}

// Static member operations
/*static*/ Vector4 Vector4::CrossProduct(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4(
		lhs.Y * rhs.Z - lhs.Z * rhs.Y,
		lhs.Z * rhs.X - lhs.X * rhs.Z,
		lhs.X * rhs.Y - lhs.Y * rhs.X,
		0.0f);
}

/*static*/ float Vector4::DotProduct(const Vector4& lhs, const Vector4& rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

/*static*/ Vector4 Vector4::Normalise(const Vector4& source)
{
	float length = source.GetLength();
	return Vector4(source.X / length, source.Y / length, source.Z / length, source.W);
}

