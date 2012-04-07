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

void Quaternion::AddScaledVector(const Vector4& vector, float scale)
{
	Quaternion q(0.0f, vector.X * scale, vector.Y * scale, vector.Z * scale);
	q = q * *this;
	W += q.W * 0.5f;
	X += q.X * 0.5f;
	Y += q.Y * 0.5f;
	Z += q.Z * 0.5f;
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