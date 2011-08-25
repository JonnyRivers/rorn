#pragma once

#include "D3DConstantBuffer.h"

using namespace Rorn::Engine;

D3DConstantBuffer::D3DConstantBuffer(ID3D11Buffer* buffer)
	: buffer_(buffer)
{
}

D3DConstantBuffer::~D3DConstantBuffer()
{
	buffer_->Release();
}