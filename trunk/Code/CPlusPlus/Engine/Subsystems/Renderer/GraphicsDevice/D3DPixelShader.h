#pragma once

#include <d3d11.h>

#include "IPixelShader.h"

namespace Rorn
{
	namespace Engine
	{
		// Simple RAII wrapper for ID3D11PixelShader*
		class D3DPixelShader : public IPixelShader
		{
		public:
			D3DPixelShader(ID3D11PixelShader* pixelShader);
			~D3DPixelShader();

			ID3D11PixelShader* GetUnderlyingShader() { return pixelShader_; }
		private:
			D3DPixelShader(D3DPixelShader&);
			D3DPixelShader& operator=(D3DPixelShader&);

			ID3D11PixelShader* pixelShader_;
		};
	}
}

