#pragma once

#include <windows.h>

#include <d3d11.h>

namespace Rorn
{
	namespace Engine
	{
		class BlitSurfaceFormat
		{
		public:
			static BlitSurfaceFormat& GetInstance();

			virtual HRESULT Initialize(ID3D11Device* device);
			virtual void Release();

			virtual void SetupGPU(
				ID3D11DeviceContext* deviceContext);
		private:
			static BlitSurfaceFormat& instance_;

			BlitSurfaceFormat(void);

			ID3D11VertexShader* vertexShader_;
			ID3D11PixelShader* pixelShader_;
			ID3D11InputLayout* vertexLayout_;
			ID3D11SamplerState* sampler_;
		};
	}
}
