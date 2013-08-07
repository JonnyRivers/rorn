#pragma once

#include <Windows.h>

#include "Interfaces/IEngine.h"

#include "Subsystems/Diagnostics/PCDiagnostics.h"
#include "Subsystems/FileSystem/PCFileSystem.h"
#include "Subsystems/Input/DirectXInputManager.h"
#include "Subsystems/Physics/BulletPhysicsSystem.h"
#include "Subsystems/Renderer/DirectXRenderer.h"
#include "Subsystems/Timekeeper/PCTimekeeper.h"

namespace Rorn
{
	namespace Engine
	{
		class PCEngine : public IEngine
		{
		public:
			static IEngine* Startup(HWND windowHandle);
			static void Shutdown(IEngine* engine);

			virtual IDiagnostics* GetDiagnostics();
			virtual IFileSystem* GetFileSystem();
			virtual const IKeyboard* GetKeyboard() const;
			virtual const IMouse* GetMouse() const;
			virtual IPhysicsSystem* GetPhysicsSystem();
			virtual IRenderer* GetRenderer();

			virtual float StartFrame();
			virtual void EndFrame();

			virtual float GetFramerate() const;// Need to move this sonewhere more sensible once we have more similar data
		private:
			PCEngine(HWND windowHandle);
			~PCEngine();

			PCEngine(PCEngine&);
			PCEngine& operator=(PCEngine&);

			PCDiagnostics diagnostics_;
			PCTimekeeper timekeeper_;
			PCFileSystem fileSystem_;
			BulletPhysicsSystem physicsSystem_;
			DirectXInputManager inputManager_;
			DirectXRenderer renderer_;
		};
	}
}