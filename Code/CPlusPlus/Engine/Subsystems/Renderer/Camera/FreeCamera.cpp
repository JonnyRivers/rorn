#pragma once

#include "FreeCamera.h"

#include "../../../../Shared/RornMaths/Constants.h"
#include "../../../../Shared/RornMaths/UnitDirection.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

FreeCamera::FreeCamera(const Position& position, const EulerAngles& eulerAngles)
	: position_(position), eulerAngles_(eulerAngles)
{
}

FreeCamera::~FreeCamera()
{
	
}

/*virtual*/ void FreeCamera::Translate(const Direction& translation)
{
	Matrix4x4 xRotationMatrix = Matrix4x4::BuildXRotationMatrix(eulerAngles_.Pitch);
	Matrix4x4 yRotationMatrix = Matrix4x4::BuildYRotationMatrix(eulerAngles_.Heading);
	UnitDirection xAxis(1.0f, 0.0f, 0.0f);
	UnitDirection yAxis(0.0f, 1.0f, 0.0f);
	UnitDirection zAxis(0.0f, 0.0f, 1.0f);
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
	UnitDirection xAxis(1.0f, 0.0f, 0.0f);
	UnitDirection yAxis(0.0f, 1.0f, 0.0f);
	UnitDirection zAxis(0.0f, 0.0f, 1.0f);
	xAxis = xAxis * xRotationMatrix;
	xAxis = xAxis * yRotationMatrix;
	yAxis = yAxis * xRotationMatrix;
	yAxis = yAxis * yRotationMatrix;
	zAxis = zAxis * xRotationMatrix;
	zAxis = zAxis * yRotationMatrix;

	// Calculate the translation
	Position negatedEyePosition = -position_;
	float translationX = Position::DotProduct(negatedEyePosition, xAxis);
	float translationY = Position::DotProduct(negatedEyePosition, yAxis);
	float translationZ = Position::DotProduct(negatedEyePosition, zAxis);

	return Matrix4x4(
		xAxis.X, yAxis.X, zAxis.X, 0.0f,
		xAxis.Y, yAxis.Y, zAxis.Y, 0.0f,
		xAxis.Z, yAxis.Z, zAxis.Z, 0.0f,
		translationX, translationY, translationZ, 1.0f);
}