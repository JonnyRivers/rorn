#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "D3DViewportWrapper.h"

using namespace Rorn::Engine;

D3DViewportWrapper::D3DViewportWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "The D3DViewportWrapper instance was created successfully." << std::endl;
}

D3DViewportWrapper::~D3DViewportWrapper()
{
	diagnostics_->GetLoggingStream() << "The D3DViewportWrapper instance is being destroyed." << std::endl;
}
