#include "Float2.h"

#include <cmath>

using namespace Rorn::Maths;

// Construction
Float2::Float2(void)
{
}

Float2::Float2(float x, float y) : X(x), Y(y)
{
}

Float2::Float2(const Float2& source) : X(source.X), Y(source.Y)
{

}

// Assignemnt
Float2& Float2::operator=(const Float2& source)
{
	X = source.X;
	Y = source.Y;

	return *this;
}
