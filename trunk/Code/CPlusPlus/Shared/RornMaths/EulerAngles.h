#pragma once

#include "Matrix4x4.h"

namespace Rorn
{
	namespace Maths
	{
		class EulerAngles
		{
		public:
			// Data
			float Heading;
			float Pitch;
			float Bank;

			// Construction
			EulerAngles(void);
			EulerAngles(float heading, float pitch, float bank);
			EulerAngles(const EulerAngles& source);

			// Assignment
			EulerAngles& operator=(const EulerAngles& source);

			// Static member operations
			static Matrix4x4 ConvertToRotationMatrix(const EulerAngles& angles);
		};
	}
}

