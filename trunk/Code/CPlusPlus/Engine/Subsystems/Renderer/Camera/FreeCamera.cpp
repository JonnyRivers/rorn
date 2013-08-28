#pragma once

#include "FreeCamera.h"

#include "../../../../Shared/RornMaths/Constants.h"
#include "../../../../Shared/RornMaths/Vector4.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

FreeCamera::FreeCamera(const Vector4& position, const EulerAngles& eulerAngles)
	: position_(position), eulerAngles_(eulerAngles)
{
}

FreeCamera::~FreeCamera()
{
	
}

/*virtual*/ void FreeCamera::GetPosition(Vector4& position)
{
	position = position_;
}

/*virtual*/ void FreeCamera::GetEyeDirection(Vector4& eyeDirection)
{
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(eulerAngles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(eulerAngles_.Heading);
	Vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);
	zAxis = zAxis * xRotationMatrix;
	zAxis = zAxis * yRotationMatrix;

	eyeDirection = zAxis;
}

/*virtual*/ void FreeCamera::GetUpVector(Vector4& up)
{
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(eulerAngles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(eulerAngles_.Heading);
	Vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
	yAxis = yAxis * xRotationMatrix;
	yAxis = yAxis * yRotationMatrix;

	up = yAxis;
}

/*virtual*/ void FreeCamera::GetRightVector(Vector4& right)
{
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(eulerAngles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(eulerAngles_.Heading);
	Vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
	xAxis = xAxis * xRotationMatrix;
	xAxis = xAxis * yRotationMatrix;

	right = xAxis;
}

/*virtual*/ void FreeCamera::Translate(const Vector4& translation)
{
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(eulerAngles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(eulerAngles_.Heading);
	Vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);
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

/*virtual*/ void FreeCamera::AlterPitch(float angle)
{
	eulerAngles_.Pitch += angle;

	if(eulerAngles_.Pitch > Pi)
		eulerAngles_.Pitch -= TwoPi;

	if(eulerAngles_.Pitch < -Pi)
		eulerAngles_.Pitch += TwoPi;
}

/*virtual*/ void FreeCamera::AlterHeading(float angle)
{
	eulerAngles_.Heading += angle;

	if(eulerAngles_.Heading > Pi)
		eulerAngles_.Heading -= TwoPi;

	if(eulerAngles_.Heading < -Pi)
		eulerAngles_.Heading += TwoPi;
}

/*virtual*/ void FreeCamera::AlterBank(float angle)
{
	eulerAngles_.Bank += angle;

	if(eulerAngles_.Bank > Pi)
		eulerAngles_.Bank -= TwoPi;

	if(eulerAngles_.Bank < -Pi)
		eulerAngles_.Bank += TwoPi;
}

/*virtual*/ Matrix4x4 FreeCamera::BuildWorldToViewMatrix() const
{
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(eulerAngles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(eulerAngles_.Heading);
	Vector4 xAxis(1.0f, 0.0f, 0.0f, 0.0f);
	Vector4 yAxis(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4 zAxis(0.0f, 0.0f, 1.0f, 0.0f);
	xAxis = xAxis * xRotationMatrix;
	xAxis = xAxis * yRotationMatrix;
	yAxis = yAxis * xRotationMatrix;
	yAxis = yAxis * yRotationMatrix;
	zAxis = zAxis * xRotationMatrix;
	zAxis = zAxis * yRotationMatrix;

	// Calculate the translation
	Vector4 negatedEyePosition = -position_;
	float translationX = Vector4::DotProduct(negatedEyePosition, xAxis);
	float translationY = Vector4::DotProduct(negatedEyePosition, yAxis);
	float translationZ = Vector4::DotProduct(negatedEyePosition, zAxis);

	return Matrix4x4(
		xAxis.X, yAxis.X, zAxis.X, 0.0f,
		xAxis.Y, yAxis.Y, zAxis.Y, 0.0f,
		xAxis.Z, yAxis.Z, zAxis.Z, 0.0f,
		translationX, translationY, translationZ, 1.0f);
}