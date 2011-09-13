#pragma once

#include "D3D11Texture2D.h"

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

using namespace Rorn::Engine;

D3D11Texture2D::D3D11Texture2D(IDiagnostics* diagnostics, ID3D11Device* device, UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory( &descDepth, sizeof(descDepth) );
	descDepth.Width = width;
	descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    HRESULT hr = device->CreateTexture2D( &descDepth, NULL, &Texture2D );
    if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create Texture2D.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create Texture2D.");
	}
	diagnostics->GetLoggingStream() << "Successfully created Texture2D" << std::endl;
}

D3D11Texture2D::~D3D11Texture2D()
{
	Texture2D->Release();
}