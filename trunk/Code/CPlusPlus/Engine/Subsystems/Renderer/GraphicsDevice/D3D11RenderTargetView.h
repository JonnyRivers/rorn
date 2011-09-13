#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11RenderTargetView
		{
		public:
			D3D11RenderTargetView(IDiagnostics* diagnostics, ID3D11Device* device, IDXGISwapChain* swapChain);
			~D3D11RenderTargetView();

			ID3D11RenderTargetView* RenderTargetView;
		private:
			D3D11RenderTargetView(D3D11RenderTargetView&);
			D3D11RenderTargetView& operator=(D3D11RenderTargetView&);
		};
	}
}

