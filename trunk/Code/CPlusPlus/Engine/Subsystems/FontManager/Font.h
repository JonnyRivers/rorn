#pragma once

#include <map>

#include <windows.h>

#include <d3d11.h>
#include <d3dx11.h>

namespace Rorn
{
	namespace Engine
	{

		class Texture
		{
		public:
			Texture();
			
			HRESULT Load(ID3D11Device* device, int dataSize, const void* const data);
			void Release();

			ID3D11ShaderResourceView* GetUnderlyingTexture();// goodbye encapsulation
		private:
			ID3D11ShaderResourceView* shaderResourceView_;
		};
	}
}