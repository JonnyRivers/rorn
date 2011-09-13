#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11DepthStencilView
		{
		public:
			D3D11DepthStencilView(IDiagnostics* diagnostics, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView, ID3D11Texture2D* texture);
			~D3D11DepthStencilView();

			ID3D11DepthStencilView* DepthStencilView;
		private:
			D3D11DepthStencilView(D3D11DepthStencilView&);
			D3D11DepthStencilView& operator=(D3D11DepthStencilView&);
		};
	}
}

