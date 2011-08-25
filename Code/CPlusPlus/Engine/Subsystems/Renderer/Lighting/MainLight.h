#pragma once

#include "../../../../Shared/RornMaths/Vector3.h"
#include "../../../../Shared/RornMaths/Float4.h"

namespace Rorn
{
	namespace Engine
	{
		class MainLight
		{
		public:
			MainLight();
			MainLight(const Rorn::Maths::Vector3& direction, const Rorn::Maths::Float4& colour);
			~MainLight();

			const Rorn::Maths::Vector3& GetDirection() const { return direction_; }
			const Rorn::Maths::Float4& GetColour() const { return colour_; }

			void SetDirection(const Rorn::Maths::Vector3& direction);
			void SetColour(const Rorn::Maths::Float4& colour);
		private:
			MainLight(MainLight&);
			MainLight& operator=(MainLight&);

			Rorn::Maths::Vector3 direction_;
			Rorn::Maths::Float4 colour_;
		};
	}
}