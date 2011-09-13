#pragma once

#include "D3D11RenderTargetView.h"

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

using namespace Rorn::Engine;

D3D11RenderTargetView::D3D11RenderTargetView(IDiagnostics* diagnostics, ID3D11Device* device, IDXGISwapChain* swapChain)
{
	ID3D11Texture2D* backBuffer = NULL;
    HRESULT hr = swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&backBuffer );
    if( FAILED( hr ) )
	{
        diagnostics->GetLoggingStream() << "Unable to retrieve swap chain back buffer.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to retrieve swap chain back buffer.");
	}

	hr = device->CreateRenderTargetView( backBuffer, NULL, &RenderTargetView );
    backBuffer->Release();
    if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create Render Target View.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create Render Target View.");
	}
	diagnostics->GetLoggingStream() << "Successfully created Render Target View" << std::endl;
}

D3D11RenderTargetView::~D3D11RenderTargetView()
{
	RenderTargetView->Release();
}