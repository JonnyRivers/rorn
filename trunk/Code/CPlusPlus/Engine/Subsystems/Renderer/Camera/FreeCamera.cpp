#pragma once

#include "FreeCamera.h"

#include "../../../../Shared/RornMaths/Constants.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

FreeCamera::FreeCamera(const Rorn::Maths::Vector3& position, const Rorn::Maths::EulerAngles& eulerAngles)
	: position_(position), eulerAngles_(eulerAngles)
{
}

FreeCamera::~FreeCamera()
{
	
}

/*virtual*/ void FreeCamera::Translate(const Vector3& translation)
{
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(eulerAngles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(eulerAngles_.Heading);
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