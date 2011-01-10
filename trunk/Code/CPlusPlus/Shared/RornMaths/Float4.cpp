#include "Float4.h"

#include <cmath>

using namespace Rorn::Maths;

// Construction
Float4::Float4(void)
{
}

Float4::Float4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w)
{
}

Float4::Float4(const Float4& source) : X(source.X), Y(source.Y), Z(source.Z), W(source.W)
{

}

// Assignemnt
Float4& Float4::operator=(const Float4& source)
{
	X = source.X;
	Y = source.Y;
	Z = source.Z;

	return *this;
}
