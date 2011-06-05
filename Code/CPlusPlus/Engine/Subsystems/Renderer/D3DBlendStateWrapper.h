#pragma once

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "ScreenCoordinates.h"

namespace Rorn
{
	namespace Engine
	{
		class D3DBlendStateWrapper
		{
		public:
			D3DBlendStateWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~D3DBlendStateWrapper();
		private:
			D3DBlendStateWrapper(D3DBlendStateWrapper&);
			D3DBlendStateWrapper& operator=(D3DBlendStateWrapper&);

			IDiagnostics* diagnostics_;
		};
	}
}
