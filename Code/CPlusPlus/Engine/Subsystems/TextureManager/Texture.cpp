#include "Texture.h"

using namespace Rorn::Engine;

Texture::Texture() : shaderResourceView_(NULL)
{
}

HRESULT Texture::Load(ID3D11Device* device, int dataSize, const void* const data)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromMemory(
		device,
		data,
		dataSize,
		NULL,
		NULL,
		&shaderResourceView_,
		NULL);

	return hr;
}

void Texture::Release()
{
	if (shaderResourceView_ != NULL)
		shaderResourceView_->Release();
}

ID3D11ShaderResourceView* Texture::GetUnderlyingTexture()
{
	return shaderResourceView_;
}