#pragma once

#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Vector3.h"

namespace Rorn
{
	namespace Engine
	{
		class PointLight
		{
		public:
			PointLight(const Maths::Vector3& position, const Maths::Float4& color, float luminosity);

			Maths::Vector3 Position;
			Maths::Float4 Color;
			float Luminosity;
		};
	}
}