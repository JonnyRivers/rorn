#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11Device
		{
		public:
			D3D11Device(IDiagnostics* diagnostics, HWND applicationWindowHandle, UINT outputWidth, UINT outputHeight);
			~D3D11Device();

			ID3D11Device* Device;
			ID3D11DeviceContext* DeviceContext;
			IDXGISwapChain* SwapChain;
		private:
			D3D11Device(D3D11Device&);
			D3D11Device& operator=(D3D11Device&);
		};
	}
}

