#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11ShaderResourceView
		{
		public:
			D3D11ShaderResourceView(IDiagnostics* diagnostics, ID3D11Device* device, unsigned int dataSize, const void* data);
			~D3D11ShaderResourceView();

			ID3D11ShaderResourceView* ShaderResourceView;
		private:
			D3D11ShaderResourceView(D3D11ShaderResourceView&);
			D3D11ShaderResourceView& operator=(D3D11ShaderResourceView&);
		};
	}
}

