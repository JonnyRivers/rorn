#pragma once

#include "../../Shared/RornMaths/Matrix4x4.h"

namespace Rorn
{
	namespace Engine
	{
		struct IModelInstance
		{
		public:
			virtual void SetInstanceToWorldMatrix(const Maths::Matrix4x4& instanceToWorldMatrix) = 0;
			virtual void SetLinearVelocity(const Maths::Vector4& linearVelocity) = 0;
		};
	}
}
