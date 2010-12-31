#pragma once

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		class UntexturedSurfaceFormat
		{
		public:
			static UntexturedSurfaceFormat& GetInstance();

			virtual HRESULT Initialize(ID3D11Device* device);
			virtual void Release();

			virtual void SetupGPU(
				ID3D11DeviceContext* deviceContext, 
				CXMMATRIX instanceToWorldMatrix, 
				CXMMATRIX worldToProjectionMatrix,
				const XMFLOAT4& ambientColor,
				const XMFLOAT4& diffuseColor,
				const XMFLOAT4& specularColor);
		private:
			static UntexturedSurfaceFormat& instance_;

			UntexturedSurfaceFormat(void);

			// This should be split up between the vertex shader and pixel shader
			struct ConstantBuffer
			{
				XMMATRIX ModelToWorldMatrix;
				XMMATRIX WorldToProjectionMatrix;
				XMFLOAT4 AmbientColor;
				XMFLOAT4 DiffuseColor;
				XMFLOAT4 SpecularColor;
				XMFLOAT4 LightDir;
				XMFLOAT4 LightColor;
			};

			ID3D11VertexShader* vertexShader_;
			ID3D11PixelShader* pixelShader_;
			ID3D11InputLayout* vertexLayout_;
			ID3D11Buffer* constantBuffer_;
		};
	}
}
