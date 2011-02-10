#pragma once

#include "../../../Shared/RornMaths/Vector3.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"

#include "Camera.h"

namespace Rorn
{
	namespace Engine
	{
		// TODO - refactor camera stuff out of the render manager
		class LookAtCamera : public Camera
		{
		public:
			LookAtCamera(const Maths::Vector3& eye, const Maths::Vector3& target, const Maths::Vector3& up);

			virtual Maths::Matrix4x4 BuildWorldToViewMatrix() const;

			// Move towards or away from the target
			void TranslateZ(float dz);
		private:
			Maths::Vector3 eye_;
			Maths::Vector3 target_;
			Maths::Vector3 up_;
		};
	}
}