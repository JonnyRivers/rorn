#pragma once

#include <map>

#include <windows.h>

#include <d3d11.h>

#include "Texture.h"

namespace Rorn
{
	namespace Engine
	{
		class TextureManager
		{
		public:
			static TextureManager& GetInstance();

			HRESULT Startup(ID3D11Device* device);
			void Shutdown();

			void Step();

			int CreateTexture(const void* data, int dataLength);
			ID3D11ShaderResourceView* GetTexture(int textureId);
		private:
			static TextureManager& instance_;

			TextureManager(void);

			ID3D11Device* device_;
			int nextTextureId_;
			std::map<int, std::unique_ptr<Texture>> textures_;
		};
	}
}