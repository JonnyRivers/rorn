#pragma once

#include "D3D11Buffer.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../../Exceptions/initialisation_exception.h"

using namespace Rorn::Engine;

D3D11Buffer::D3D11Buffer(IDiagnostics* diagnostics, ID3D11Device* device, D3D11_BIND_FLAG bindFlag, unsigned int dataSize, const void* initialData /*= NULL*/)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = dataSize;
	bd.BindFlags = bindFlag;
	bd.CPUAccessFlags = 0;
	HRESULT hr;
	if(initialData != NULL)
	{
		D3D11_SUBRESOURCE_DATA indexResourceData;
		ZeroMemory( &indexResourceData, sizeof(indexResourceData) );
		indexResourceData.pSysMem = initialData;
		hr = device->CreateBuffer( &bd, &indexResourceData, &Buffer );
	}
	else
	{
		hr = device->CreateBuffer( &bd, NULL, &Buffer );
	}
	
    if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create buffer.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
        throw initialisation_exception("Unable to create buffer.");
	}
}

D3D11Buffer::~D3D11Buffer()
{
	Buffer->Release();
}