#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11Buffer
		{
		public:
			D3D11Buffer(IDiagnostics* diagnostics, ID3D11Device* device, D3D11_BIND_FLAG bindFlag, unsigned int dataSize, const void* initialData = NULL);
			~D3D11Buffer();

			ID3D11Buffer* Buffer;
		private:
			D3D11Buffer(D3D11Buffer&);
			D3D11Buffer& operator=(D3D11Buffer&);
		};
	}
}

