#pragma once

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "ScreenCoordinates.h"

namespace Rorn
{
	namespace Engine
	{
		class D3DDepthStencilViewWrapper
		{
		public:
			D3DDepthStencilViewWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~D3DDepthStencilViewWrapper();
		private:
			D3DDepthStencilViewWrapper(D3DDepthStencilViewWrapper&);
			D3DDepthStencilViewWrapper& operator=(D3DDepthStencilViewWrapper&);

			IDiagnostics* diagnostics_;
		};
	}
}
