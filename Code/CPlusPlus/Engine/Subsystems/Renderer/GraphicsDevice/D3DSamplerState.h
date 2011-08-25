#pragma once

#include <d3d11.h>

#include "ISamplerState.h"

namespace Rorn
{
	namespace Engine
	{
		// Simple RAII wrapper for ID3D11SamplerState*
		class D3DSamplerState : public ISamplerState
		{
		public:
			D3DSamplerState(ID3D11SamplerState* samplerState);
			~D3DSamplerState();

			ID3D11SamplerState* GetUnderlyingSamplerState() { return samplerState_; }
		private:
			D3DSamplerState(D3DSamplerState&);
			D3DSamplerState& operator=(D3DSamplerState&);

			ID3D11SamplerState* samplerState_;
		};
	}
}

