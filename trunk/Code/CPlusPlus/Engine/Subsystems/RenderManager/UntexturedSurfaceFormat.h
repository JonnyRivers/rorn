#pragma once

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

#include "SurfaceFormat.h"

namespace Rorn
{
	namespace Engine
	{
		class UntexturedSurfaceFormat : public SurfaceFormat
		{
		public:
			UntexturedSurfaceFormat(void);
			~UntexturedSurfaceFormat(void);

			virtual HRESULT Startup(ID3D11Device* device);
			virtual void Shutdown();

			virtual void SetupGPU(ID3D11DeviceContext* deviceContext, 
				CXMMATRIX instanceToWorldMatrix, CXMMATRIX worldToProjectionMatrix) const;
		private:
			struct SimpleVertex
			{
				XMFLOAT3 Position;
				XMFLOAT3 Normal;
				XMFLOAT4 Color;
			};

			struct ConstantBuffer
			{
				XMMATRIX ModelToWorldMatrix;
				XMMATRIX WorldToProjectionMatrix;
			};

			ID3D11VertexShader* vertexShader_;
			ID3D11PixelShader* pixelShader_;
			ID3D11InputLayout* vertexLayout_;
			ID3D11Buffer* constantBuffer_;
		};
	}
}
