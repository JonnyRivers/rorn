#include "LookToCamera.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

LookToCamera::LookToCamera(const Vector3& eye, const Vector3& direction, const Vector3& up)
	: eye_(eye), direction_(direction), up_(up)
{
}

/*virtual*/ Matrix4x4 LookToCamera::BuildWorldToViewMatrix() const
{
	// Calculate the three camera space axes
	Vector3 cameraZAxis = Vector3::GetUnitVector(direction_);
	Vector3 cameraXAxis = Vector3::GetUnitVector(Vector3::CrossProduct(up_, cameraZAxis));
	Vector3 cameraYAxis = Vector3::CrossProduct(cameraZAxis, cameraXAxis);
	
	// Calculate the translation
	Vector3 negatedEyePosition = -eye_;
	float translationX = Vector3::DotProduct(cameraXAxis, negatedEyePosition);
	float translationY = Vector3::DotProduct(cameraYAxis, negatedEyePosition);
	float translationZ = Vector3::DotProduct(cameraZAxis, negatedEyePosition);

	return Matrix4x4(
		cameraXAxis.X, cameraYAxis.X, cameraZAxis.X, 0.0f,
		cameraXAxis.Y, cameraYAxis.Y, cameraZAxis.Y, 0.0f,
		cameraXAxis.Z, cameraYAxis.Z, cameraZAxis.Z, 0.0f,
		translationX, translationY, translationZ, 1.0f);
}

void LookToCamera::Translate(const Vector3& translation)
{
	// Calculate the three camera space axes
	Vector3 zAxis = Vector3::GetUnitVector(direction_);
	Vector3 xAxis = Vector3::GetUnitVector(Vector3::CrossProduct(up_, zAxis));
	Vector3 yAxis = Vector3::CrossProduct(zAxis, xAxis);

	eye_ += translation.X * xAxis;
	eye_ += translation.Y * yAxis;
	eye_ += translation.Z * zAxis;
}

void LookToCamera::AlterPitch(float angle)
{
	// rotate up about the cross of up_ and direction_
	Vector3 zAxis = Vector3::GetUnitVector(direction_);
	Vector3 xAxis = Vector3::GetUnitVector(Vector3::CrossProduct(up_, zAxis));
	Matrix4x4 rotationMatrix = Matrix4x4::BuildRotationMatrix(xAxis, angle);
	up_ = Vector3::GetUnitVector(up_ * rotationMatrix);
	direction_ = Vector3::GetUnitVector(direction_ * rotationMatrix);
}

void LookToCamera::AlterHeading(float angle)
{
	// rotate the direction about up
	Matrix4x4 rotationMatrix = Matrix4x4::BuildRotationMatrix(up_, angle);
	direction_ = Vector3::GetUnitVector(direction_ * rotationMatrix);
}
