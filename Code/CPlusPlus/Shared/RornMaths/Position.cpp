#include "Position.h"

#include <cmath>

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
Position& Position::operator+=(const Position& rhs)
{
	X += rhs.X;
	Y += rhs.Y;
	Z += rhs.Z;

	return *this;
}

Position& Position::operator-=(const Position& rhs)
{
	X -= rhs.X;
	Y -= rhs.Y;
	Z -= rhs.Z;

	return *this;
}

Position& Position::operator*=(float rhs)
{
	X *= rhs;
	Y *= rhs;
	Z *= rhs;

	return *this;
}

Position& Position::operator/=(float rhs)
{
	float rhsReciprocal = 1.0f / rhs;

	X *= rhsReciprocal;
	Y *= rhsReciprocal;
	Z *= rhsReciprocal;

	return *this;
}

// Member operations
float Position::GetLength() const
{
	return sqrt(X * X + Y * Y + Z * Z);
}


