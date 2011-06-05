#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "D3DBlendStateWrapper.h"

using namespace Rorn::Engine;

D3DBlendStateWrapper::D3DBlendStateWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "The D3DBlendStateWrapper instance was created successfully." << std::endl;
}

D3DBlendStateWrapper::~D3DBlendStateWrapper()
{
	diagnostics_->GetLoggingStream() << "The D3DBlendStateWrapper instance is being destroyed." << std::endl;
}
