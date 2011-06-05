#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "D3DRenderTargetViewWrapper.h"

using namespace Rorn::Engine;

D3DRenderTargetViewWrapper::D3DRenderTargetViewWrapper(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "The D3DRenderTargetViewWrapper instance was created successfully." << std::endl;
}

D3DRenderTargetViewWrapper::~D3DRenderTargetViewWrapper()
{
	diagnostics_->GetLoggingStream() << "The D3DRenderTargetViewWrapper instance is being destroyed." << std::endl;
}
