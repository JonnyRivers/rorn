#pragma once

#include "IDiagnostics.h"
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
			virtual IDiagnostics* GetDiagnostics() = 0;
			virtual const IKeyboard* GetKeyboard() const = 0;
			virtual const IMouse* GetMouse() const = 0;
			virtual IRenderer* GetRenderer() = 0;
		};
	}
}