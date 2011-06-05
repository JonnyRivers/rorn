#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "D3DDepthStencilTextureWrapper.h"

using namespace Rorn::Engine;

D3DDepthStencilTextureWrapper::D3DDepthStencilTextureWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "The D3DDepthStencilTextureWrapper instance was created successfully." << std::endl;
}

D3DDepthStencilTextureWrapper::~D3DDepthStencilTextureWrapper()
{
	diagnostics_->GetLoggingStream() << "The D3DDepthStencilTextureWrapper instance is being destroyed." << std::endl;
}
