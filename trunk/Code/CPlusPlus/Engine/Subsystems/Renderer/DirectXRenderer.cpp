#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "DirectXRenderer.h"

using namespace Rorn::Engine;

DirectXRenderer::DirectXRenderer(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: applicationWindowHandle_(applicationWindowHandle),
	  diagnostics_(diagnostics),
	  screenCoordinates_(applicationWindowHandle, diagnostics),
	  device_(applicationWindowHandle, diagnostics),
	  renderTarget_(applicationWindowHandle, diagnostics),
	  depthStencilTexture_(applicationWindowHandle, diagnostics),
	  depthStencilView_(applicationWindowHandle, diagnostics),
	  blendState_(applicationWindowHandle, diagnostics),
	  viewport_(applicationWindowHandle, diagnostics)
{
	diagnostics_->GetLoggingStream() << "The DirectXRenderer subsystem was created successfully." << std::endl;
}

DirectXRenderer::~DirectXRenderer()
{
	diagnostics_->GetLoggingStream() << "The DirectXRenderer subsystem is being destroyed." << std::endl;
}
