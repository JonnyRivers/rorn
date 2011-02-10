#pragma once

#include "../../../Shared/RornMaths/Vector3.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"

#include "Camera.h"

namespace Rorn
{
	namespace Engine
	{
		// TODO - refactor camera stuff out of the render manager
		class FreeCamera : public Camera
		{
		public:
			FreeCamera(const Maths::Vector3& position, const Maths::Vector3& direction, const Maths::Vector3& up);

			virtual Maths::Matrix4x4 BuildWorldToViewMatrix() const;

			void Translate(const Maths::Vector3& translation);
			void RotateX(float angle);
			void RotateY(float angle);
		private:
			Maths::Vector3 position_;
			Maths::Vector3 xAxis_;
			Maths::Vector3 yAxis_;
			Maths::Vector3 zAxis_;
		};
	}
}