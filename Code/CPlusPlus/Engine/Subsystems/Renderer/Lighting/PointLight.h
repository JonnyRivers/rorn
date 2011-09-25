#pragma once

#include "../../../../Shared/RornMaths/Position.h"
#include "../../../../Shared/RornMaths/Float4.h"

namespace Rorn
{
	namespace Engine
	{
		class PointLight
		{
		public:
			PointLight(const Rorn::Maths::Position& position, const Rorn::Maths::Float4& colour, float luminosity);
			~PointLight();

			const Rorn::Maths::Position& GetPosition() const { return position_; }
			const Rorn::Maths::Float4& GetColour() const { return colour_; }
			float GetLuminosity() const { return luminosity_; }
		private:
			PointLight(PointLight&);
			PointLight& operator=(PointLight&);

			Rorn::Maths::Position position_;
			Rorn::Maths::Float4 colour_;
			float luminosity_;
		};
	}
}