#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11DeviceWrapper
		{
		public:
			D3D11DeviceWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics, UINT outputWidth, UINT outputHeight);
			~D3D11DeviceWrapper();

			ID3D11Device* Device;
			ID3D11DeviceContext* DeviceContext;
			IDXGISwapChain* SwapChain;
		private:
			D3D11DeviceWrapper(D3D11DeviceWrapper&);
			D3D11DeviceWrapper& operator=(D3D11DeviceWrapper&);

			IDiagnostics* diagnostics_;
		};
	}
}

