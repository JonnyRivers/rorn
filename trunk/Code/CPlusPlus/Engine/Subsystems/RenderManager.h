#pragma once

#include <windows.h>
#include <d3d11.h>

namespace Rorn
{
	namespace Engine
	{
		class RenderManager
		{
		public:
			static RenderManager& GetInstance();

			void Startup(HWND hwnd);
			void Shutdown();

			void Step();
		private:
			static RenderManager& instance_;

			RenderManager(void) {}

			ID3D11Device* device_;
			ID3D11DeviceContext* deviceContext_;
			IDXGISwapChain* swapChain_;
			ID3D11RenderTargetView* renderTargetView_;
			D3D_DRIVER_TYPE driverType_;
			D3D_FEATURE_LEVEL featureLevel_;
		};
	}
}