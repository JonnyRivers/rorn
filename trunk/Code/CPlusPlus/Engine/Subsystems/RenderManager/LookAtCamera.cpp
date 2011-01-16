#include "LookAtCamera.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

LookAtCamera::LookAtCamera(const Vector3& eye, const Vector3& target, const Vector3& up)
	: eye_(eye), target_(target), up_(up)
{
}

void LookAtCamera::TranslateX(float dx)
{
	Vector3 eyeDirection = target_ - eye_;
	Vector3 cameraZAxis = Vector3::GetUnitVector(eyeDirection);
	Vector3 cameraXAxis = Vector3::GetUnitVector(Vector3::CrossProduct(up_, cameraZAxis));

	eye_ += (cameraXAxis * dx);
}

/*virtual*/ Matrix4x4 LookAtCamera::BuildWorldToViewMatrix() const
{
	Vector3 eyeDirection = target_ - eye_;

	// Calculate the three camera space axes
	Vector3 cameraZAxis = Vector3::GetUnitVector(eyeDirection);
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