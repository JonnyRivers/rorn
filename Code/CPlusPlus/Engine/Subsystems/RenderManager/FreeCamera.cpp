#include "FreeCamera.h"

#include "../../../Shared/RornMaths/Matrix4x4.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

FreeCamera::FreeCamera(const Vector3& position, const Vector3& direction, const Vector3& up)
	: position_(position)
{
	zAxis_ = Vector3::GetUnitVector(direction);
	xAxis_ = Vector3::GetUnitVector(Vector3::CrossProduct(up, zAxis_));
	yAxis_ = Vector3::CrossProduct(zAxis_, xAxis_);
}

/*virtual*/ Matrix4x4 FreeCamera::BuildWorldToViewMatrix() const
{
	// Calculate the translation
	Vector3 negatedEyePosition = -position_;
	float translationX = Vector3::DotProduct(xAxis_, negatedEyePosition);
	float translationY = Vector3::DotProduct(yAxis_, negatedEyePosition);
	float translationZ = Vector3::DotProduct(zAxis_, negatedEyePosition);

	return Matrix4x4(
		xAxis_.X, yAxis_.X, zAxis_.X, 0.0f,
		xAxis_.Y, yAxis_.Y, zAxis_.Y, 0.0f,
		xAxis_.Z, yAxis_.Z, zAxis_.Z, 0.0f,
		translationX, translationY, translationZ, 1.0f);
}

void FreeCamera::Translate(const Vector3& translation)
{
	position_ += translation.X * xAxis_;
	position_ += translation.Y * yAxis_;
	position_ += translation.Z * zAxis_;
}

void FreeCamera::RotateX(float angle)
{
	Matrix4x4 rotationMatrix = Matrix4x4::BuildRotationMatrix(xAxis_, angle);
	yAxis_ = yAxis_ * rotationMatrix;
	zAxis_ = zAxis_ * rotationMatrix;
}

void FreeCamera::RotateY(float angle)
{
	Matrix4x4 rotationMatrix = Matrix4x4::BuildRotationMatrix(yAxis_, angle);
	xAxis_ = xAxis_ * rotationMatrix;
	zAxis_ = zAxis_ * rotationMatrix;
}