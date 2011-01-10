#pragma once

#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Vector3.h"

namespace Rorn
{
	namespace Engine
	{
		class Light
		{
		public:
			Light(const Maths::Vector3& direction, const Maths::Float4& color);

			Maths::Vector3 Direction;
			Maths::Float4 Color;
		};
	}
}