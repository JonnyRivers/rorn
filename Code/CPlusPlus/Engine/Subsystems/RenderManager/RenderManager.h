#pragma once

#include <list>
#include <map>

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

#include "SurfaceFormat.h"

namespace Rorn
{
	namespace Engine
	{
		class Camera;
		class Model;
		class ModelInstance;

		class RenderManager
		{
		public:
			static RenderManager& GetInstance();

			HRESULT Startup(HWND hwnd);
			void Shutdown();

			Camera& CreateCamera(XMVECTOR eye, XMVECTOR target, XMVECTOR up);
			void SetCurrentCamera(Camera& camera);

			void Step();
		private:
			static RenderManager& instance_;

			RenderManager(void);

			void SetupScreenCoordinates(HWND hwnd);
			HRESULT SetupDeviceAndSwapChain(HWND hwnd);
			HRESULT SetupRenderTargetView();
			void SetupViewport();
			HRESULT SetupSurfaceFormats();

			ID3D11Device* device_;
			ID3D11DeviceContext* deviceContext_;
			IDXGISwapChain* swapChain_;
			ID3D11RenderTargetView* renderTargetView_;
			D3D_DRIVER_TYPE driverType_;
			D3D_FEATURE_LEVEL featureLevel_;

			UINT outputWidth_;
			UINT outputHeight_;
			FLOAT aspectRatio_;

			std::list<Camera> cameras_;
			Camera* currentCamera_;

			// Geometry
			std::map<SurfaceFormat::Type, std::unique_ptr<SurfaceFormat>> surfaceFormats_;
			std::list<Model> models_;
			std::list<ModelInstance>  modelInstances_;
		};
	}
}