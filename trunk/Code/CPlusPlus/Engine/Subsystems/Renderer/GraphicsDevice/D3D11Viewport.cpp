#pragma once

#include "D3D11Viewport.h"

using namespace Rorn::Engine;

D3D11Viewport::D3D11Viewport(IDiagnostics* diagnostics, ID3D11DeviceContext* deviceContext, UINT width, UINT height)
{
	// Setup the viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<FLOAT>(width);
	viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    deviceContext->RSSetViewports( 1, &viewport );

	diagnostics->GetLoggingStream() << "Successfully set Device Context viewport" << std::endl;
}
