#pragma once

#include "D3D11BlendState.h"

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

using namespace Rorn::Engine;

D3D11BlendState::D3D11BlendState(IDiagnostics* diagnostics, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	D3D11_BLEND_DESC blendStateDescription;
	memset(&blendStateDescription, 0, sizeof(blendStateDescription));
    blendStateDescription.RenderTarget[0].BlendEnable = true;
    blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = device->CreateBlendState(&blendStateDescription, &BlendState);
	if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create Blend State.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create Blend State.");
	}
	diagnostics->GetLoggingStream() << "Successfully created Blend State" << std::endl;

	float blendFactor[] = { 1.0f,1.0f,1.0f,1.0f };
    UINT sampleMask   = 0xffffffff;
    deviceContext->OMSetBlendState(BlendState, blendFactor, sampleMask);
}

D3D11BlendState::~D3D11BlendState()
{
	BlendState->Release();
}