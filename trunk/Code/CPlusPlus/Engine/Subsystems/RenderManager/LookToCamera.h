#pragma once

#include "../../../Shared/RornMaths/Vector3.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"

#include "Camera.h"

namespace Rorn
{
	namespace Engine
	{
		// TODO - refactor camera stuff out of the render manager
		class LookToCamera : public Camera
		{
		public:
			LookToCamera(const Maths::Vector3& eye, const Maths::Vector3& direction, const Maths::Vector3& up);

			virtual Maths::Matrix4x4 BuildWorldToViewMatrix() const;

			void Translate(const Maths::Vector3& translation);
			void AlterPitch(float angle);
			void AlterHeading(float angle);
		private:
			Maths::Vector3 eye_;
			Maths::Vector3 direction_;
			Maths::Vector3 up_;
		};
	}
}