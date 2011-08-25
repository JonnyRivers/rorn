#pragma once

#include "D3DPixelShader.h"

using namespace Rorn::Engine;

D3DPixelShader::D3DPixelShader(ID3D11PixelShader* pixelShader)
	: pixelShader_(pixelShader)
{
}

D3DPixelShader::~D3DPixelShader()
{
	pixelShader_->Release();
}