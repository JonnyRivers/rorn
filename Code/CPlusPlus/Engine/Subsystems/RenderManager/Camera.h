#pragma once

#include "../../../Shared/RornMaths/Matrix4x4.h"

namespace Rorn
{
	namespace Engine
	{
		// TODO - refactor camera stuff out of the render manager
		class Camera
		{
		public:
			Camera();

			virtual Maths::Matrix4x4 BuildWorldToViewMatrix() const = 0;
		};
	}
}