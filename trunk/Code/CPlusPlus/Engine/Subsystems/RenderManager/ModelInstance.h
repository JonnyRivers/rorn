#pragma once

#include <d3d11.h>

#include "../../../Shared/RornMaths/Matrix4x4.h"

namespace Rorn
{
	namespace Engine
	{
		class Model;

		class ModelInstance
		{
		public:
			ModelInstance(const Model* model, const Maths::Matrix4x4& instanceToWorldMatrix);
			~ModelInstance(void);

			void Draw(ID3D11DeviceContext* deviceContext, const Maths::Matrix4x4& worldToProjectionMatrix) const;

			void RotateY(float angle);
		private:
			const Model* model_;
			Maths::Matrix4x4 instanceToWorldMatrix_;
		};
	}
}
