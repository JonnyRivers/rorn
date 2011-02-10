#include "FreeCamera.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

FreeCamera::FreeCamera(const Vector3& position, const EulerAngles& angles)
	: position_(position), angles_(angles)
{
}

/*virtual*/ Matrix4x4 FreeCamera::BuildWorldToViewMatrix() const
{
	Matrix4x4 rotationMatrix = EulerAngles::ConvertToRotationMatrix(angles_);
	Vector3 xAxis(rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13);
	Vector3 yAxis(rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23);
	Vector3 zAxis(rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33);

	// Calculate the translation
	Vector3 negatedEyePosition = -position_;
	float translationX = Vector3::DotProduct(xAxis, negatedEyePosition);
	float translationY = Vector3::DotProduct(yAxis, negatedEyePosition);
	float translationZ = Vector3::DotProduct(zAxis, negatedEyePosition);

	return Matrix4x4(
		xAxis.X, yAxis.X, zAxis.X, 0.0f,
		xAxis.Y, yAxis.Y, zAxis.Y, 0.0f,
		xAxis.Z, yAxis.Z, zAxis.Z, 0.0f,
		translationX, translationY, translationZ, 1.0f);
}

void FreeCamera::Translate(const Vector3& translation)
{
	Matrix4x4 rotationMatrix = EulerAngles::ConvertToRotationMatrix(angles_);
	Vector3 xAxis(rotationMatrix.M11, rotationMatrix.M12, rotationMatrix.M13);
	Vector3 yAxis(rotationMatrix.M21, rotationMatrix.M22, rotationMatrix.M23);
	Vector3 zAxis(rotationMatrix.M31, rotationMatrix.M32, rotationMatrix.M33);

	position_ += translation.X * xAxis;
	position_ += translation.Y * yAxis;
	position_ += translation.Z * zAxis;
}

void FreeCamera::AlterPitch(float angle)
{
	angles_.Pitch += angle;
}

void FreeCamera::AlterHeading(float angle)
{
	angles_.Heading += angle;
}

void FreeCamera::AlterBank(float angle)
{
	angles_.Bank += angle;
}