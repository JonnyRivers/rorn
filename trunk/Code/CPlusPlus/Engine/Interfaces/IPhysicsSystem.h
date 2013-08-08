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
			virtual void GetBoundsInstanceToWorldTransform(unsigned int boundsInstanceId, Maths::Matrix4x4& instanceToWorldMatrix) const = 0;

			virtual void SetBoundsInstanceLinearVelocity(unsigned int boundsInstanceId, const Maths::Vector4& linearVelocity) = 0;
			virtual void SetBoundsInstanceToWorldTransform(unsigned int boundsInstanceId, const Maths::Matrix4x4& instanceToWorldMatrix) = 0;

			virtual unsigned int CreateBoundsInstance(unsigned int boundsId, const Maths::Matrix4x4& instanceToWorldMatrix) = 0;
			virtual unsigned int LoadBounds(unsigned int numPhysicsPrimitives, StreamReader& fileReader) = 0;
			virtual void SetEnabled(bool enabled) = 0;
		};
	}
}