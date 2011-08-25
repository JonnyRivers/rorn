#pragma once

#include "D3DIndexBuffer.h"

using namespace Rorn::Engine;

D3DIndexBuffer::D3DIndexBuffer(ID3D11Buffer* buffer)
	: buffer_(buffer)
{
}

D3DIndexBuffer::~D3DIndexBuffer()
{
	buffer_->Release();
}