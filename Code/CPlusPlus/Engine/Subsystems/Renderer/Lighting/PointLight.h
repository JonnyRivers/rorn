#pragma once

#include "../../../../Shared/RornMaths/Vector4.h"
#include "../../../../Shared/RornMaths/Float4.h"

namespace Rorn
{
	namespace Engine
	{
		class PointLight
		{
		public:
			PointLight(const Rorn::Maths::Vector4& position, const Rorn::Maths::Float4& colour, float luminosity);
			~PointLight();

			const Rorn::Maths::Vector4& GetPosition() const { return position_; }
			const Rorn::Maths::Float4& GetColour() const { return colour_; }
			float GetLuminosity() const { return luminosity_; }
		private:
			PointLight(PointLight&);
			PointLight& operator=(PointLight&);

			Rorn::Maths::Vector4 position_;
			Rorn::Maths::Float4 colour_;
			float luminosity_;
		};
	}
}