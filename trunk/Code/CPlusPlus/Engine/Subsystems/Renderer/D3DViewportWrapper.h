#pragma once

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "ScreenCoordinates.h"

namespace Rorn
{
	namespace Engine
	{
		class D3DViewportWrapper
		{
		public:
			D3DViewportWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~D3DViewportWrapper();
		private:
			D3DViewportWrapper(D3DViewportWrapper&);
			D3DViewportWrapper& operator=(D3DViewportWrapper&);

			IDiagnostics* diagnostics_;
		};
	}
}
