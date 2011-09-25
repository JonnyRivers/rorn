#include "Position.h"

#include <cmath>

#include "Direction.h"
#include "UnitDirection.h"

using namespace Rorn::Maths;

// Construction
Position::Position(void)
{
}

Position::Position(float x, float y, float z) : X(x), Y(y), Z(z), W(1.0f)
{
}

Position::Position(const Position& source) : X(source.X), Y(source.Y), Z(source.Z), W(source.W)
{

}

// Assignemnt
Position& Position::operator=(const Position& source)
{
	X = source.X;
	Y = source.Y;
	Z = source.Z;
	W = source.W;

	return *this;
}

// In-place binary operations
Position& Position::operator+=(const Direction& rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	Z += rhs.Z;

	return *this;
}

Position& Position::operator-=(const Direction& rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	Z -= rhs.Z;

	return *this;
}

// Member operations
float Position::GetLength() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}

// Static member operations
/*static*/ float Position::DotProduct(const Position& lhs, const UnitDirection& rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;// Assumes that rhs.W is 0
}
