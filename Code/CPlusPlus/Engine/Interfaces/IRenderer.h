#pragma once

#include "../../Shared/RornMaths/EulerAngles.h"
#include "../../Shared/RornMaths/Float4.h"
#include "../../Shared/RornMaths/Matrix4x4.h"
#include "../../Shared/RornMaths/Position.h"

#include "ICamera.h"
#include "IModel.h"
#include "IModelInstance.h"

namespace Rorn
{
	namespace Engine
	{
		struct IRenderer
		{
		public:
			// Model interface
			virtual unsigned int LoadModel(const wchar_t* modelPathName) = 0;
			virtual IModel* GetModel(unsigned int modelId) = 0;
			virtual unsigned int CreateModelInstance(unsigned int modelId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix) = 0;
			virtual IModelInstance* GetModelInstance(unsigned int modelInstanceId) = 0;

			// Camera interface
			virtual unsigned int CreateFreeCamera(const Rorn::Maths::Position& position, const Rorn::Maths::EulerAngles& eulerAngles) = 0;
			virtual void SetCurrentCamera(unsigned int cameraId) = 0;
			virtual ICamera* GetCamera(unsigned int cameraId) = 0;

			// Light interface
			virtual void SetAmbientLight(const Rorn::Maths::Float4& colour) = 0;
			virtual void SetMainLight(const Rorn::Maths::UnitDirection& direction, const Rorn::Maths::Float4& colour) = 0;
			virtual unsigned int CreatePointLight(const Rorn::Maths::Position& position, const Rorn::Maths::Float4& colour, float luminosity) = 0;

			// Debug rendering interface
			virtual void AddDebugText(const char* text, float x, float y) = 0;
		};
	}
}