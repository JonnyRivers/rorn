#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11BlendState
		{
		public:
			D3D11BlendState(IDiagnostics* diagnostics, ID3D11Device* device, ID3D11DeviceContext* deviceContext);
			~D3D11BlendState();

			ID3D11BlendState* BlendState;
		private:
			D3D11BlendState(D3D11BlendState&);
			D3D11BlendState& operator=(D3D11BlendState&);
		};
	}
}

