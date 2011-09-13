#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11Texture2D
		{
		public:
			D3D11Texture2D(IDiagnostics* diagnostics, ID3D11Device* device, UINT width, UINT height);
			~D3D11Texture2D();

			ID3D11Texture2D* Texture2D;
		private:
			D3D11Texture2D(D3D11Texture2D&);
			D3D11Texture2D& operator=(D3D11Texture2D&);
		};
	}
}

