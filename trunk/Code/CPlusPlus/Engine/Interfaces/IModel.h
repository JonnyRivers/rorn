#pragma once

#include "../../Shared/RornMaths/BoundingBox.h"

namespace Rorn
{
	namespace Engine
	{
		struct IModel
		{
		public:
			virtual const Rorn::Maths::BoundingBox& GetBoundingBox() = 0;
		};
	}
}