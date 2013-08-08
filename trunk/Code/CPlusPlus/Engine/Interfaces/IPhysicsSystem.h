#pragma once

#include "../../Shared/RornMaths/Matrix4x4.h"

namespace Rorn
{
	namespace Engine
	{
		class StreamReader;

		struct IPhysicsSystem
		{
		public:
			virtual unsigned int CreateBoundsInstance(unsigned int boundsId, const Maths::Matrix4x4& instanceToWorldMatrix) = 0;
			virtual void GetBoundsInstanceToWorldTransform(unsigned int boundsInstanceId, Maths::Matrix4x4& instanceToWorldMatrix) = 0;
			virtual unsigned int LoadBounds(unsigned int numPhysicsPrimitives, StreamReader& fileReader) = 0;
			virtual void SetEnabled(bool enabled) = 0;
		};
	}
}