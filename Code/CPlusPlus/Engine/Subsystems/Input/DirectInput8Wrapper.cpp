#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "DirectInput8Wrapper.h"

using namespace Rorn::Engine;

DirectInput8Wrapper::DirectInput8Wrapper(IDiagnostics* diagnostics) : diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "DirectInput8Wrapper instance is being created." << std::endl;

	HRESULT hr;
	if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), 0x0800,
                                         IID_IDirectInput8, ( VOID** )&directInput_, NULL ) ) )
    {
		diagnostics->GetLoggingStream() << "Unable to create DirectInput subsystem.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create DirectInput subsystem");
	}

	diagnostics_->GetLoggingStream() << "DirectInput8Wrapper instance was created successfully." << std::endl;
}

DirectInput8Wrapper::~DirectInput8Wrapper()
{
	diagnostics_->GetLoggingStream() << "DirectInput8Wrapper instance is being destroyed." << std::endl;

	if(directInput_ != NULL)
		directInput_->Release();

	diagnostics_->GetLoggingStream() << "DirectInput8Wrapper instance was destroyed successfully." << std::endl;
}

DirectInput8Wrapper::operator IDirectInput8*()
{
	return directInput_;
}