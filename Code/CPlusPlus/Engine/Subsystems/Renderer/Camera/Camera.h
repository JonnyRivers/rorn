#pragma once

#include "../../../Interfaces/ICamera.h"

#include "../../../../Shared/RornMaths/Matrix4x4.h"

namespace Rorn
{
	namespace Engine
	{
		class Camera : public ICamera
		{
		public:
			Camera();
			~Camera();

			virtual Rorn::Maths::Matrix4x4 BuildWorldToViewMatrix() const = 0;
		private:
			Camera(Camera&);
			Camera& operator=(Camera&);
		};
	}
}