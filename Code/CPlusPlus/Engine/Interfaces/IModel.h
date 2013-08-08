#pragma once

#include "../../Shared/RornMaths/BoundingBox.h"

namespace Rorn
{
	namespace Engine
	{
		struct IModel
		{
		public:
			virtual unsigned int GetBoundsId() const = 0;
			virtual const Rorn::Maths::BoundingBox& GetBoundingBox() const = 0;
		};
	}
}