#pragma once

#include <vector>

#include "../../../../Shared/RornMaths/Matrix4x4.h"

#include "../../../Interfaces/IModelInstance.h"

namespace Rorn
{
	namespace Engine
	{
		class ModelInstance : public IModelInstance
		{
		public:
			ModelInstance(unsigned int modelId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix);
			~ModelInstance();

			virtual void RotateY(float angle);

			unsigned int GetModelId() const { return modelId_; }
			const Maths::Matrix4x4& GetInstanceToWorldMatrix() const { return instanceToWorldMatrix_; }
		private:
			ModelInstance(ModelInstance&);
			ModelInstance& operator=(ModelInstance&);

			unsigned int modelId_;
			Rorn::Maths::Matrix4x4 instanceToWorldMatrix_;
		};
	}
}