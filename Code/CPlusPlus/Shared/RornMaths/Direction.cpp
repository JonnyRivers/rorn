#include "Direction.h"

#include <cmath>

#include "UnitDirection.h"

using namespace Rorn::Maths;

// Construction
Direction::Direction(void)
{
}

Direction::Direction(float x, float y, float z) : X(x), Y(y), Z(z), W(0.0f)
{
}

Direction::Direction(const Direction& source) : X(source.X), Y(source.Y), Z(source.Z), W(source.W)
{

}

// Assignemnt
Direction& Direction::operator=(const Direction& source)
{
	X = source.X;
	Y = source.Y;
	Z = source.Z;
	W = source.W;

	return *this;
}

// In-place binary operations
Direction& Direction::operator+=(const Direction& rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	Z += rhs.Z;

	return *this;
}

Direction& Direction::operator-=(const Direction& rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	Z -= rhs.Z;

	return *this;
}

Direction& Direction::operator*=(float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;

	return *this;
}

Direction& Direction::operator/=(float rhs)
{
	float rhsReciprocal = 1.0f / rhs;

	X *= rhsReciprocal;
	Y *= rhsReciprocal;
	Z *= rhsReciprocal;

	return *this;
}

// Member operations
float Direction::GetLength() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

// Static member operations
/*static*/ Direction Direction::CrossProduct(const Direction& lhs, const Direction& rhs)
{
	return Direction(
		lhs.Y * rhs.Z - lhs.Z * rhs.Y,
		lhs.Z * rhs.X - lhs.X * rhs.Z,
		lhs.X * rhs.Y - lhs.Y * rhs.X);
}

/*static*/ float Direction::DotProduct(const Direction& lhs, const Direction& rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}

/*static*/ UnitDirection Direction::Normalise(const Direction& source)
{
	float length = source.GetLength();
	return UnitDirection(source.X / length, source.Y / length, source.Z / length);
}

