#include "OrientationQuaternion.h"

#include <cmath>

#include "UnitDirection.h"

using namespace Rorn::Maths;

OrientationQuaternion::OrientationQuaternion(float w, float x, float y, float z) : W(w), X(x), Y(y), Z(z)
{
}

OrientationQuaternion::OrientationQuaternion(const UnitDirection& axis, float theta)
{
	float halfTheta = theta * 0.5f;
	W = cos(theta * 0.5f);
	float sinHalfTheta = sin(halfTheta);
	X = axis.X * sinHalfTheta;
	Y = axis.Y * sinHalfTheta;
	Z = axis.Z * sinHalfTheta;
}