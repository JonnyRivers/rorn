#pragma once

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IRenderer.h"

#include "../../Interfaces/IDiagnostics.h"

#include "ScreenCoordinates.h"
#include "D3DBlendStateWrapper.h"
#include "D3DDepthStencilTextureWrapper.h"
#include "D3DDepthStencilViewWrapper.h"
#include "D3DDeviceWrapper.h"
#include "D3DRenderTargetViewWrapper.h"
#include "D3DViewportWrapper.h"

namespace Rorn
{
	namespace Engine
	{
		class DirectXRenderer : public IRenderer
		{
		public:
			DirectXRenderer(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~DirectXRenderer();
		private:
			DirectXRenderer(DirectXRenderer&);
			DirectXRenderer& operator=(DirectXRenderer&);

			HWND applicationWindowHandle_;
			IDiagnostics* diagnostics_;

			ScreenCoordinates screenCoordinates_;
			D3DDeviceWrapper device_;
			D3DRenderTargetViewWrapper renderTarget_;
			D3DDepthStencilTextureWrapper depthStencilTexture_;
			D3DDepthStencilViewWrapper depthStencilView_;
			D3DBlendStateWrapper blendState_;
			D3DViewportWrapper viewport_;
		};
	}
}