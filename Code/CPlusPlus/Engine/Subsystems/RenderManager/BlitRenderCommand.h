#pragma once

#include <d3d11.h>

#include "../../../Shared/RornMaths/Float2.h"
#include "../../../Shared/RornMaths/Float4.h"

namespace Rorn
{
	namespace Engine
	{
		class BlitRenderCommand
		{
		public:
			struct VertexFormat
			{
				Maths::Float4 Position;
				Maths::Float2 DiffuseUV;
			};

			BlitRenderCommand(ID3D11Device* device, int textureId);

			void UpdateData(ID3D11DeviceContext* deviceContext, unsigned int vertexCount, const VertexFormat* vertexData, unsigned int indexCount, const int* indexData);

			void Draw(ID3D11DeviceContext* deviceContext);

			void Release();

			static const unsigned int MaxNumVerts = 1024;
		private:
			int textureId_;

			int vertexCount_;
			int vertexDataSize_;
			ID3D11Buffer* vertexBuffer_;
			int indexCount_;
			int indexDataSize_;
			ID3D11Buffer* indexBuffer_;
		};
	}
}
