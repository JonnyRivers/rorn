#pragma once

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		class Model;

		class ModelInstance
		{
		public:
			ModelInstance(const Model& model, CXMMATRIX instanceToWorldMatrix_);
			~ModelInstance(void);

			void Draw(ID3D11DeviceContext* deviceContext, CXMMATRIX worldToProjectionMatrix) const;
		
		private:
			const Model& model_;
			XMMATRIX instanceToWorldMatrix_;
		};
	}
}
