#include "LightingManager.h"

#include <cassert>

#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Vector3.h"

#include "../DiagnosticsManager/DiagnosticsManager.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

/*static*/ LightingManager& LightingManager::instance_ = LightingManager();// init static instance

/*static*/ LightingManager& LightingManager::GetInstance()
{
	return instance_;
}

LightingManager::LightingManager(void)
	: ambientLightColor_(0.0f, 0.0f, 0.0f, 1.0f), 
	  mainLight_( Vector3(0.0f, 0.0f, 1.0f), Float4(0.0f, 0.0f, 0.0f, 1.0f) )
{
}

HRESULT LightingManager::Startup()
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Lighting Manager is starting up." << std::endl;

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Lighting Manager started up successfully." << std::endl;

	return S_OK;
}

void LightingManager::Shutdown()
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Lighting Manager is shutting down." << std::endl;

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Lighting Manager shut down successfully." << std::endl;
}

void LightingManager::Step()
{
}

const Float4& LightingManager::GetAmbientLightColor() const
{
	return ambientLightColor_;
}

void LightingManager::SetAmbientLightColor(const Float4& color)
{
	ambientLightColor_ = color;
}

void LightingManager::SetUpMainLight(const Vector3& direction, const Float4& color)
{
	mainLight_.Direction = direction;
	mainLight_.Color = color;
}

const DirectionalLight& LightingManager::GetMainLight() const
{
	return mainLight_;
}
