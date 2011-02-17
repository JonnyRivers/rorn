#include "EulerAngles.h"

#include <cmath>

#include "Matrix4x4.h"

using namespace Rorn::Maths;

// Construction
EulerAngles::EulerAngles(void)
{
}

EulerAngles::EulerAngles(float heading, float pitch, float bank)
	: Heading(heading), Pitch(pitch), Bank(bank)
{
}

EulerAngles::EulerAngles(const EulerAngles& source)
	: Heading(source.Heading), Pitch(source.Pitch), Bank(source.Bank)
{
}

// Assignment
EulerAngles& EulerAngles::operator=(const EulerAngles& source)
{
	Heading = source.Heading;
	Bank = source.Bank;
	Pitch = source.Pitch;

	return *this;
}

// Static member operations
/*static*/ Matrix4x4 EulerAngles::ConvertToRotationMatrix(const EulerAngles& angles)
{
	float sinHeading = sin(angles.Heading);
	float cosHeading = cos(angles.Heading);
	float sinPitch = sin(angles.Pitch);
	float cosPitch = cos(angles.Pitch);
	float sinBank = sin(angles.Bank);
	float cosBank = cos(angles.Bank);

	return Matrix4x4(
		cosHeading * cosBank + sinHeading * sinPitch * sinBank,
		-cosHeading * sinBank + sinHeading * sinPitch * cosBank,
		sinHeading * cosPitch,
		0.0f,
		sinBank * cosPitch,
		cosBank * cosPitch,
		-sinPitch,
		0.0f,
		-sinHeading * cosBank + cosHeading * sinPitch * sinBank,
		sinBank * sinHeading + cosHeading * sinPitch * cosBank,
		cosHeading * cosPitch,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f);
}

