#pragma once

#include <vector>

#include "../../../../Shared/RornMaths/Matrix4x4.h"

#include "../../../Interfaces/IModelInstance.h"

#include "../../../Interfaces/IPhysicsSystem.h"

namespace Rorn
{
	namespace Engine
	{
		class ModelInstance : public IModelInstance
		{
		public:
			ModelInstance(unsigned int modelId, IPhysicsSystem* physicsSystem, unsigned int boundsInstanceId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix);
			ModelInstance(unsigned int modelId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix);
			~ModelInstance();

			const Maths::Matrix4x4& GetInstanceToWorldMatrix();
			unsigned int GetModelId() const { return modelId_; }

			virtual void SetInstanceToWorldMatrix(const Maths::Matrix4x4& instanceToWorldMatrix);
			
		private:
			ModelInstance(ModelInstance&);
			ModelInstance& operator=(ModelInstance&);

			unsigned int modelId_;
			Rorn::Maths::Matrix4x4 instanceToWorldMatrix_;

			bool hasBounds_;
			IPhysicsSystem* physicsSystem_;
			unsigned int boundsInstanceId_;
		};
	}
}