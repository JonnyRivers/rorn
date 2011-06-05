#pragma once

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "ScreenCoordinates.h"

namespace Rorn
{
	namespace Engine
	{
		class D3DDepthStencilTextureWrapper
		{
		public:
			D3DDepthStencilTextureWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~D3DDepthStencilTextureWrapper();
		private:
			D3DDepthStencilTextureWrapper(D3DDepthStencilTextureWrapper&);
			D3DDepthStencilTextureWrapper& operator=(D3DDepthStencilTextureWrapper&);

			IDiagnostics* diagnostics_;
		};
	}
}
