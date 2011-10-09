#pragma once

#include <map>

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IRenderer.h"

#include "../../Interfaces/IDiagnostics.h"
#include "../../Interfaces/IFileSystem.h"

#include "GraphicsDevice/D3DGraphicsDevice.h"

#include "Material/Material.h"
#include "Material/MaterialType.h"

#include "Model/Model.h"
#include "Model/ModelInstance.h"
#include "Model/RenderCommand.h"

#include "Camera/Camera.h"
#include "Camera/FreeCamera.h"

#include "Font/Font.h"

#include "Lighting/AmbientLight.h"
#include "Lighting/MainLight.h"
#include "Lighting/PointLight.h"

namespace Rorn
{
	namespace Engine
	{
		class DirectXRenderer : public IRenderer
		{
		public:
			DirectXRenderer(HWND applicationWindowHandle, IDiagnostics* diagnostics, IFileSystem* fileSystem);
			~DirectXRenderer();

			void StartFrame();
			void Draw();

			// Model interface
			virtual unsigned int LoadModel(const wchar_t* modelPathName);
			virtual IModel* GetModel(unsigned int modelId);
			virtual unsigned int CreateModelInstance(unsigned int modelId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix);
			virtual IModelInstance* GetModelInstance(unsigned int modelInstanceId);

			// Camera interface
			virtual unsigned int CreateFreeCamera(const Maths::Vector4& position, const Rorn::Maths::EulerAngles& eulerAngles);
			virtual void SetCurrentCamera(unsigned int cameraId);
			virtual ICamera* GetCamera(unsigned int cameraId);

			// Light interface
			virtual void SetAmbientLight(const Rorn::Maths::Float4& colour);
			virtual void SetMainLight(const Maths::Vector4& direction, const Rorn::Maths::Float4& colour);
			virtual unsigned int CreatePointLight(const Maths::Vector4& position, const Maths::Float4& colour, float luminosity);

			// Debug rendering interface
			virtual void AddDebugText(const char* text, float x, float y);
		private:
			DirectXRenderer(DirectXRenderer&);
			DirectXRenderer& operator=(DirectXRenderer&);

			// Draw implementation
			static Maths::Matrix4x4 BuildViewToProjectionMatrix(float fovAngle, float aspectRatio, float nearClipZ, float farClipZ);

			void DrawModelInstance(unsigned int modelInstanceId, const Maths::Matrix4x4& worldToViewMatrix, const Maths::Matrix4x4& worldToProjectionMatrix);
			void DrawModel(unsigned int modelId, const Maths::Matrix4x4& instanceToWorldMatrix, const Maths::Matrix4x4& worldToViewMatrix, const Maths::Matrix4x4& worldToProjectionMatrix);
			void DrawRenderCommand(unsigned int renderCommandId, const Maths::Matrix4x4& instanceToWorldMatrix, const Maths::Matrix4x4& worldToViewMatrix, const Maths::Matrix4x4& worldToProjectionMatrix);

			// Debug rendering implementation
			unsigned int CreateDebugTextRenderCommand();

			// Font interface
			unsigned int LoadFont(const wchar_t* fontPathName);

			// Data
			IDiagnostics* diagnostics_;
			IFileSystem* fileSystem_;
			D3DGraphicsDevice graphicsDevice_;

			// Material data
			std::map<MaterialType::Type, std::unique_ptr<Material>> materials_;

			// Model data
			std::map<unsigned int, std::unique_ptr<Model>> models_;
			std::map<unsigned int, std::unique_ptr<RenderCommand>> renderCommands_;
			std::map<unsigned int, std::unique_ptr<ModelInstance>> modelInstances_;

			// Camera data
			std::map<unsigned int, std::unique_ptr<Camera>> cameras_;
			unsigned int currentCameraId_;

			// Light data
			AmbientLight ambientLight_;
			MainLight mainLight_;
			std::map<unsigned int, std::unique_ptr<PointLight>> pointLights_;

			// Font data
			std::map<unsigned int, std::unique_ptr<Font>> fonts_;
			unsigned int debugTextFontId_;

			// Debug renderering data
			static const unsigned int maxNumDebugTextRenderCommands_ = 16;
			static const unsigned int maxNumDebugTextRenderCommandVerts_ = 256 * 6;// 2 triangles per glyph
			unsigned int debugTextItemsThisFrame_;
			std::vector<unsigned int> debugTextRenderCommandIds_;
			std::vector<unsigned int> debugRenderCommandsIdsToDraw_;
		};
	}
}