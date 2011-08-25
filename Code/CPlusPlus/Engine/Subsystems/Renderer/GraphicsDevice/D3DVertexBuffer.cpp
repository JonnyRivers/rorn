#pragma once

#include "D3DVertexBuffer.h"

using namespace Rorn::Engine;

D3DVertexBuffer::D3DVertexBuffer(ID3D11Buffer* buffer)
	: buffer_(buffer)
{
}

D3DVertexBuffer::~D3DVertexBuffer()
{
	buffer_->Release();
}