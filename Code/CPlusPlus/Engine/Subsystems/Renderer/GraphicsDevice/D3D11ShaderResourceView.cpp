#pragma once

#include "D3D11ShaderResourceView.h"

#include <d3dx11async.h>

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../../Exceptions/initialisation_exception.h"

using namespace Rorn::Engine;

D3D11ShaderResourceView::D3D11ShaderResourceView(IDiagnostics* diagnostics, ID3D11Device* device, unsigned int dataSize, const void* data)
{
	HRESULT hr = ::D3DX11CreateShaderResourceViewFromMemory(
		device,
		data,
		dataSize,
		NULL,
		NULL,
		&ShaderResourceView,
		NULL);

	if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create shader resource view.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
        throw initialisation_exception("Unable to create shader resource view.");
	}
}

D3D11ShaderResourceView::~D3D11ShaderResourceView()
{
	ShaderResourceView->Release();
}