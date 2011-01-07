#pragma once

#include <list>
#include <map>

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		class Camera;
		class Light;
		class Model;
		class ModelInstance;

		// This is getting big already.  Need a strategy to break it up...
		class RenderManager
		{
		public:
			static RenderManager& GetInstance();

			HRESULT Startup(HWND hwnd);
			void Shutdown();

			Camera* CreateCamera(XMVECTOR eye, XMVECTOR target, XMVECTOR up);
			void SetCurrentCamera(Camera* camera);

			XMFLOAT4 GetAmbientLightColor();
			void SetAmbientLightColor(const XMFLOAT4& color);

			Light* CreateLight(const XMFLOAT4& direction, const XMFLOAT4& color);
			Light* GetMainLight();
			void SetMainLight(Light* light);

			Model* LoadOrGetModel(const wchar_t* modelPathName);
			ModelInstance* CreateModelInstance(Model* model, CXMMATRIX instanceToWorldMatrix);

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
			ID3D11Texture2D* depthStencil_;
			ID3D11DepthStencilView* depthStencilView_;
			D3D_DRIVER_TYPE driverType_;
			D3D_FEATURE_LEVEL featureLevel_;

			UINT outputWidth_;
			UINT outputHeight_;
			FLOAT aspectRatio_;

			// Cameras
			std::list<std::unique_ptr<Camera>> cameras_;
			Camera* currentCamera_;

			// Lights
			XMFLOAT4 ambientLightColor_;
			std::list<std::unique_ptr<Light>> lights_;
			Light* mainLight_;

			// Geometry
			std::list<std::unique_ptr<Model>> models_;
			std::list<std::unique_ptr<ModelInstance>>  modelInstances_;
		};
	}
}