#pragma once

#include "D3D11DepthStencilView.h"

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

using namespace Rorn::Engine;

D3D11DepthStencilView::D3D11DepthStencilView(IDiagnostics* diagnostics, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView, ID3D11Texture2D* texture)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    descDSV.Texture2D.MipSlice = 0;
	HRESULT hr = device->CreateDepthStencilView( texture, &descDSV, &DepthStencilView );
    if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create Depth Stencil View.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create Depth Stencil View.");
	}
	diagnostics->GetLoggingStream() << "Successfully created Depth Stencil View" << std::endl;

	deviceContext->OMSetRenderTargets( 1, &renderTargetView, DepthStencilView );
}

D3D11DepthStencilView::~D3D11DepthStencilView()
{
	DepthStencilView->Release();
}