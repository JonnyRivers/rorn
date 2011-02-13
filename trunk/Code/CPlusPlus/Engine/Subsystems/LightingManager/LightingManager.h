#pragma once

#include <list>

#include <windows.h>

#include "DirectionalLight.h"
#include "PointLight.h"

namespace Rorn
{
	namespace Engine
	{
		class LightingManager
		{
		public:
			static LightingManager& GetInstance();

			HRESULT Startup();
			void Shutdown();

			void Step();

			const Maths::Float4& GetAmbientLightColor() const;
			void SetAmbientLightColor(const Maths::Float4& color);
			void SetUpMainLight(const Maths::Vector3& direction, const Maths::Float4& color);
			const DirectionalLight& GetMainLight() const;
		private:
			static LightingManager& instance_;

			LightingManager(void);

			Maths::Float4 ambientLightColor_;
			DirectionalLight mainLight_;
			std::list<PointLight> pointLights_;
		};
	}
}