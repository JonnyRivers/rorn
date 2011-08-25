#pragma once

#include "D3DSamplerState.h"

using namespace Rorn::Engine;

D3DSamplerState::D3DSamplerState(ID3D11SamplerState* samplerState)
	: samplerState_(samplerState)
{
}

D3DSamplerState::~D3DSamplerState()
{
	samplerState_->Release();
}