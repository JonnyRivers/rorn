#include "FreeCamera.h"

#include "../../../Shared/RornMaths/Constants.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

FreeCamera::FreeCamera(const Vector3& position, const EulerAngles& angles)
	: position_(position), angles_(angles)
{
}

/*virtual*/ Matrix4x4 FreeCamera::BuildWorldToViewMatrix() const
{
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(angles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(angles_.Heading);
	Vector3 xAxis(1.0f, 0.0f, 0.0f);
	Vector3 yAxis(0.0f, 1.0f, 0.0f);
	Vector3 zAxis(0.0f, 0.0f, 1.0f);
	xAxis = xAxis * xRotationMatrix;
	xAxis = xAxis * yRotationMatrix;
	yAxis = yAxis * xRotationMatrix;
	yAxis = yAxis * yRotationMatrix;
	zAxis = zAxis * xRotationMatrix;
	zAxis = zAxis * yRotationMatrix;

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
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(angles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(angles_.Heading);
	Vector3 xAxis(1.0f, 0.0f, 0.0f);
	Vector3 yAxis(0.0f, 1.0f, 0.0f);
	Vector3 zAxis(0.0f, 0.0f, 1.0f);
	xAxis = xAxis * xRotationMatrix;
	xAxis = xAxis * yRotationMatrix;
	yAxis = yAxis * xRotationMatrix;
	yAxis = yAxis * yRotationMatrix;
	zAxis = zAxis * xRotationMatrix;
	zAxis = zAxis * yRotationMatrix;

	position_ += translation.X * xAxis;
	position_ += translation.Y * yAxis;
	position_ += translation.Z * zAxis;
}

void FreeCamera::AlterPitch(float angle)
{
	angles_.Pitch += angle;

	if(angles_.Pitch > Pi)
		angles_.Pitch -= TwoPi;

	if(angles_.Pitch < -Pi)
		angles_.Pitch += TwoPi;
}

void FreeCamera::AlterHeading(float angle)
{
	angles_.Heading += angle;

	if(angles_.Heading > Pi)
		angles_.Heading -= TwoPi;

	if(angles_.Heading < -Pi)
		angles_.Heading += TwoPi;
}

void FreeCamera::AlterBank(float angle)
{
	angles_.Bank += angle;

	if(angles_.Bank > Pi)
		angles_.Bank -= TwoPi;

	if(angles_.Bank < -Pi)
		angles_.Bank += TwoPi;
}