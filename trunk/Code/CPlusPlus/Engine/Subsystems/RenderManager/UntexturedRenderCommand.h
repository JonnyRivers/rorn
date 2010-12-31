#pragma once

#include "RenderCommand.h"

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

#include "../FileManager/FileReader.h"

namespace Rorn
{
	namespace Engine
	{
		class UntexturedRenderCommand : public RenderCommand
		{
		public:
			UntexturedRenderCommand(void);
			~UntexturedRenderCommand(void);

			HRESULT LoadFromFile(FileReader& fileReader, ID3D11Device* device);

			virtual void Draw(ID3D11DeviceContext* deviceContext, 
				CXMMATRIX instanceToWorldMatrix, CXMMATRIX worldToProjectionMatrix);

			virtual void Release();
		private:
			struct VertexFormat
			{
				XMFLOAT3 Position;
				XMFLOAT3 Normal;
			};

			XMFLOAT4 ambientColor_;
			XMFLOAT4 diffuseColor_;
			XMFLOAT4 specularColor_;

			int vertexCount_;
			int vertexDataSize_;
			ID3D11Buffer* vertexBuffer_;
			int indexCount_;
			int indexDataSize_;
			ID3D11Buffer* indexBuffer_;
		};
	}
}

