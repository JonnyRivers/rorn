#pragma once

#include <list>
#include <map>

#include <windows.h>

#include <d3d11.h>

#include "../../../Shared/RornMaths/EulerAngles.h"
#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"
#include "../../../Shared/RornMaths/Vector3.h"

namespace Rorn
{
	namespace Engine
	{
		class Camera;
		class FreeCamera;
		class Light;
		class LookAtCamera;
		class LookToCamera;
		class Model;
		class ModelInstance;

		// This is getting big already.  Need a strategy to break it up...
		class RenderManager
		{
		public:
			static RenderManager& GetInstance();

			HRESULT Startup(HWND hwnd);
			void Shutdown();

			LookAtCamera* CreateLookAtCamera(const Maths::Vector3& eye, const Maths::Vector3& target, const Maths::Vector3& up);
			LookToCamera* CreateLookToCamera(const Maths::Vector3& position, const Maths::Vector3& direction, const Maths::Vector3& up);
			Maths::Vector3 GetCurrentCameraEyeDir() const;
			void SetCurrentCamera(Camera* camera);

			Maths::Float4 GetAmbientLightColor() const;
			void SetAmbientLightColor(const Maths::Float4& color);

			Light* CreateLight(const Maths::Vector3& direction, const Maths::Float4& color);
			Light* GetMainLight();
			void SetMainLight(Light* light);

			Model* LoadOrGetModel(const wchar_t* modelPathName);
			ModelInstance* CreateModelInstance(Model* model, const Maths::Matrix4x4& instanceToWorldMatrix);

			void Step();
		private:
			static RenderManager& instance_;

			RenderManager(void);

			void SetupScreenCoordinates(HWND hwnd);
			HRESULT SetupDeviceAndSwapChain(HWND hwnd);
			HRESULT SetupRenderTargetView();
			void SetupViewport();
			HRESULT SetupSurfaceFormats();

			Maths::Matrix4x4 BuildViewToProjectionMatrix(
				float fovAngle, float aspectRatio, float nearClipZ, float farClipZ) const;

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
			Maths::Float4 ambientLightColor_;
			std::list<std::unique_ptr<Light>> lights_;
			Light* mainLight_;

			// Geometry
			std::list<std::unique_ptr<Model>> models_;
			std::list<std::unique_ptr<ModelInstance>>  modelInstances_;
		};
	}
}