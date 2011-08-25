#pragma once

#include <d3d11.h>

#include "ITexture.h"

namespace Rorn
{
	namespace Engine
	{
		// Simple RAII wrapper for ID3D11ShaderResourceView*
		class D3DTexture : public ITexture
		{
		public:
			D3DTexture(ID3D11ShaderResourceView* texture);
			~D3DTexture();

			ID3D11ShaderResourceView* GetUnderlyingTexture() { return texture_; }
		private:
			D3DTexture(D3DTexture&);
			D3DTexture& operator=(D3DTexture&);

			ID3D11ShaderResourceView* texture_;
		};
	}
}

