#include "OrientationQuaternion.h"

#include <cmath>

#include "UnitDirection.h"

using namespace Rorn::Maths;

OrientationQuaternion::OrientationQuaternion(float w, float x, float y, float z) : W(w), X(x), Y(y), Z(z)
{
}

float OrientationQuaternion::GetLength() const
{
	return sqrt(W * W + X * X + Y * Y + Z * Z);
}

/*static*/ OrientationQuaternion OrientationQuaternion::Normalise(const OrientationQuaternion& source)
{
	float length = source.GetLength();
	return OrientationQuaternion(source.W / length, source.X / length, source.Y / length, source.Z / length);
}

void OrientationQuaternion::ToAxisAngle(UnitDirection& axis, float& angle)
{
	angle = acosf(W);

	float sinAngleReciprocal = 1.0f/sinf(angle);
	axis = UnitDirection(
		X * sinAngleReciprocal,
		Y * sinAngleReciprocal,
		Z * sinAngleReciprocal);

	angle *= 2;
}

/*static*/ OrientationQuaternion OrientationQuaternion::CreateFromAxisAngle(const UnitDirection& axis, float angle)
{
	float halfAngle = angle * 0.5f;
	float sinHalfAngle = sin(halfAngle);

	return OrientationQuaternion(
		cos(halfAngle),
		axis.X * sinHalfAngle,
		axis.Y * sinHalfAngle,
		axis.Z * sinHalfAngle);
}