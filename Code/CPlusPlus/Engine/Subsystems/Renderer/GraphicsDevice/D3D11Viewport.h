#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11Viewport
		{
		public:
			D3D11Viewport(IDiagnostics* diagnostics, ID3D11DeviceContext* deviceContext, UINT width, UINT height);
		private:
			D3D11Viewport(D3D11Viewport&);
			D3D11Viewport& operator=(D3D11Viewport&);
		};
	}
}

