#pragma once

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "ScreenCoordinates.h"

namespace Rorn
{
	namespace Engine
	{
		class D3DRenderTargetViewWrapper
		{
		public:
			D3DRenderTargetViewWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~D3DRenderTargetViewWrapper();
		private:
			D3DRenderTargetViewWrapper(D3DRenderTargetViewWrapper&);
			D3DRenderTargetViewWrapper& operator=(D3DRenderTargetViewWrapper&);

			IDiagnostics* diagnostics_;
		};
	}
}
