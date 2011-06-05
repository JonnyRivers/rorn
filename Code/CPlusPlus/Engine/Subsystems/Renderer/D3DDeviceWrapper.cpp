#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "D3DDeviceWrapper.h"

using namespace Rorn::Engine;

D3DDeviceWrapper::D3DDeviceWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "The D3DDeviceWrapper instance was created successfully." << std::endl;
}

D3DDeviceWrapper::~D3DDeviceWrapper()
{
	diagnostics_->GetLoggingStream() << "The D3DDeviceWrapper instance is being destroyed." << std::endl;
}
