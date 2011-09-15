#pragma once

#include "D3D11SamplerState.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../../Exceptions/initialisation_exception.h"

using namespace Rorn::Engine;

D3D11SamplerState::D3D11SamplerState(IDiagnostics* diagnostics, ID3D11Device* device)
{
	D3D11_SAMPLER_DESC samplerDescription;
    ZeroMemory( &samplerDescription, sizeof(samplerDescription) );
    samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDescription.MinLOD = 0;
    samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

    HRESULT hr = device->CreateSamplerState( &samplerDescription, &SamplerState );
    if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to sampler state.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
        throw initialisation_exception("Unable to sampler state.");
	}
}

D3D11SamplerState::~D3D11SamplerState()
{
	SamplerState->Release();
}