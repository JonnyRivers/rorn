#include "Quaternion.h"

#include <cmath>

#include "Vector4.h"

using namespace Rorn::Maths;

Quaternion::Quaternion() : W(1.0f), X(0.0f), Y(0.0f), Z(0.0f)
{
}

Quaternion::Quaternion(float w, float x, float y, float z) : W(w), X(x), Y(y), Z(z)
{
}

float Quaternion::GetLength() const
{
	return sqrt(W * W + X * X + Y * Y + Z * Z);
}

/*static*/ Quaternion Quaternion::Normalise(const Quaternion& source)
{
	float length = source.GetLength();
	return Quaternion(source.W / length, source.X / length, source.Y / length, source.Z / length);
}

void Quaternion::ToAxisAngle(Vector4& axis, float& angle)
{
	angle = acosf(W);

	float sinAngleReciprocal = 1.0f/sinf(angle);
	axis = Vector4(
		X * sinAngleReciprocal,
		Y * sinAngleReciprocal,
		Z * sinAngleReciprocal,
		0.0f);

	angle *= 2;
}

/*static*/ Quaternion Quaternion::CreateFromAxisAngle(const Vector4& axis, float angle)
{
	float halfAngle = angle * 0.5f;
	float sinHalfAngle = sin(halfAngle);

	return Quaternion(
		cos(halfAngle),
		axis.X * sinHalfAngle,
		axis.Y * sinHalfAngle,
		axis.Z * sinHalfAngle);
}