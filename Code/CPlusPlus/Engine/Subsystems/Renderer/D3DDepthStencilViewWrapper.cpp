#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "D3DDepthStencilViewWrapper.h"

using namespace Rorn::Engine;

D3DDepthStencilViewWrapper::D3DDepthStencilViewWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "The D3DDepthStencilViewWrapper instance was created successfully." << std::endl;
}

D3DDepthStencilViewWrapper::~D3DDepthStencilViewWrapper()
{
	diagnostics_->GetLoggingStream() << "The D3DDepthStencilViewWrapper instance is being destroyed." << std::endl;
}
