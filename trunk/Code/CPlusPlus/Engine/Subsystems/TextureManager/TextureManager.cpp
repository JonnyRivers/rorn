#include "TextureManager.h"

#include <cassert>

#include "../DiagnosticsManager/DiagnosticsManager.h"

using namespace Rorn::Engine;

/*static*/ TextureManager& TextureManager::instance_ = TextureManager();// init static instance

/*static*/ TextureManager& TextureManager::GetInstance()
{
	return instance_;
}

TextureManager::TextureManager(void)
	: device_(NULL), nextTextureId_(1)
{
}

HRESULT TextureManager::Startup(ID3D11Device* device)
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Texture Manager is starting up." << std::endl;

	device_ = device;
	device_->AddRef();

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Texture Manager started up successfully." << std::endl;

	return S_OK;
}

void TextureManager::Shutdown()
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Texture Manager is shutting down." << std::endl;

	std::map<int, std::unique_ptr<Texture>>::iterator textureIter;
	for(textureIter = textures_.begin() ; textureIter != textures_ .end() ; ++textureIter)
		textureIter->second->Release();
	textures_.clear();

	if(device_ != NULL)
		device_->Release();

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Texture Manager shut down successfully." << std::endl;
}

void TextureManager::Step()
{
}

int TextureManager::CreateTexture(const void* data, int dataLength)
{
	Rorn::Engine::Texture* newTexture = new Rorn::Engine::Texture();
	newTexture->Load(device_, dataLength, data);
	textures_.insert(
		std::make_pair<int, std::unique_ptr<Texture>>(
			nextTextureId_, std::unique_ptr<Texture>(newTexture)));

	return nextTextureId_++;
}

ID3D11ShaderResourceView* TextureManager::GetTexture(int textureId)
{
	return textures_.find(textureId)->second->GetUnderlyingTexture();
}
