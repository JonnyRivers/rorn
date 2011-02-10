#pragma once

#include "../../../Shared/RornMaths/EulerAngles.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"
#include "../../../Shared/RornMaths/Vector3.h"

#include "Camera.h"

namespace Rorn
{
	namespace Engine
	{
		// TODO - refactor camera stuff out of the render manager
		class FreeCamera : public Camera
		{
		public:
			FreeCamera(const Maths::Vector3& position, const Maths::EulerAngles& angles);

			virtual Maths::Matrix4x4 BuildWorldToViewMatrix() const;

			void Translate(const Maths::Vector3& translation);
			void AlterPitch(float angle);
			void AlterHeading(float angle);
			void AlterBank(float angle);
		private:
			Maths::Vector3 position_;
			Maths::EulerAngles angles_;
		};
	}
}