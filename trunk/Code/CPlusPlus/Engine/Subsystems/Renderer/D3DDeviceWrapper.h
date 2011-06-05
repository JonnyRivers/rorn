#pragma once

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "ScreenCoordinates.h"

namespace Rorn
{
	namespace Engine
	{
		class D3DDeviceWrapper
		{
		public:
			D3DDeviceWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~D3DDeviceWrapper();
		private:
			D3DDeviceWrapper(D3DDeviceWrapper&);
			D3DDeviceWrapper& operator=(D3DDeviceWrapper&);

			IDiagnostics* diagnostics_;
		};
	}
}
