#include "UnitDirection.h"

#include <cmath>

using namespace Rorn::Maths;

// Construction
UnitDirection::UnitDirection(void)
{
}

UnitDirection::UnitDirection(float x, float y, float z) : X(x), Y(y), Z(z), W(0.0f)
{
}

UnitDirection::UnitDirection(const UnitDirection& source) : X(source.X), Y(source.Y), Z(source.Z), W(source.W)
{

}

// Assignemnt
UnitDirection& UnitDirection::operator=(const UnitDirection& source)
{
	X = source.X;
	Y = source.Y;
	Z = source.Z;
	W = source.W;

	return *this;
}

// Static member operations
/*static*/ UnitDirection UnitDirection::CrossProduct(const UnitDirection& lhs, const UnitDirection& rhs)
{
	return UnitDirection(
		lhs.Y * rhs.Z - lhs.Z * rhs.Y,
		lhs.Z * rhs.X - lhs.X * rhs.Z,
		lhs.X * rhs.Y - lhs.Y * rhs.X);
}

/*static*/ float UnitDirection::DotProduct(const UnitDirection& lhs, const UnitDirection& rhs)
{
	return lhs.X * rhs.X + lhs.Y * rhs.Y + lhs.Z * rhs.Z;
}


