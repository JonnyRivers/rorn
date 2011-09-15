#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11SamplerState
		{
		public:
			D3D11SamplerState(IDiagnostics* diagnostics, ID3D11Device* device);
			~D3D11SamplerState();

			ID3D11SamplerState* SamplerState;
		private:
			D3D11SamplerState(D3D11SamplerState&);
			D3D11SamplerState& operator=(D3D11SamplerState&);
		};
	}
}

