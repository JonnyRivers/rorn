#pragma once

#include "D3DTexture.h"

using namespace Rorn::Engine;

D3DTexture::D3DTexture(ID3D11ShaderResourceView* texture)
	: texture_(texture)
{
}

D3DTexture::~D3DTexture()
{
	texture_->Release();
}