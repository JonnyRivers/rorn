#pragma once

#include "IDiagnostics.h"
#include "IFileSystem.h"
#include "IKeyboard.h"
#include "IMouse.h"
#include "IRenderer.h"

namespace Rorn
{
	namespace Engine
	{
		struct IEngine
		{
		public:
			virtual ~IEngine() {}

			virtual IDiagnostics* GetDiagnostics() = 0;
			virtual IFileSystem* GetFileSystem() = 0;
			virtual const IKeyboard* GetKeyboard() const = 0;
			virtual const IMouse* GetMouse() const = 0;
			virtual IRenderer* GetRenderer() = 0;

			virtual float StartFrame() = 0;
			virtual void EndFrame() = 0;

			// Need to move this sonewhere more sensible once we have more similar data
			virtual float GetFramerate() const = 0;
		};
	}
}