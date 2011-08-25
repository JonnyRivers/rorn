#pragma once

#include "../../../../Shared/RornMaths/Float4.h"

namespace Rorn
{
	namespace Engine
	{
		class AmbientLight
		{
		public:
			AmbientLight();
			AmbientLight(const Rorn::Maths::Float4& colour);
			~AmbientLight();

			const Rorn::Maths::Float4& GetColour() const { return colour_; }
			void SetColour(const Rorn::Maths::Float4& colour);
		private:
			AmbientLight(AmbientLight&);
			AmbientLight& operator=(AmbientLight&);

			Rorn::Maths::Float4 colour_;
		};
	}
}