#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "DirectXInputManager.h"

using namespace Rorn::Engine;

DirectXInputManager::DirectXInputManager(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: keyboard_(NULL), 
	  mouse_(NULL),
	  diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "DirectXInputManager instance is being created." << std::endl;

	HRESULT hr;
	if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), 0x0a00,
                                         IID_IDirectInput8, ( VOID** )&directInputSubsystem_, NULL ) ) )
    {
		diagnostics->GetLoggingStream() << "Unable to create DirectInput subsystem.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create DirectInput subsystem");
	}

	diagnostics->GetLoggingStream() << "DirectXInputManager successfully created the DirectInput subsystem." << std::endl;

	keyboard_ = new DirectXKeyboard(applicationWindowHandle, diagnostics, directInputSubsystem_);
	mouse_ = new DirectXMouse(applicationWindowHandle, diagnostics, directInputSubsystem_);

	diagnostics_->GetLoggingStream() << "DirectXInputManager instance was created successfully." << std::endl;
}

DirectXInputManager::~DirectXInputManager()
{
	diagnostics_->GetLoggingStream() << "DirectXInputManager instance is being destroyed." << std::endl;

	delete mouse_;
	delete keyboard_;

	if(directInputSubsystem_ != NULL)
		directInputSubsystem_->Release();

	diagnostics_->GetLoggingStream() << "DirectXInputManager instance was destroyed successfully." << std::endl;
}

const IKeyboard* DirectXInputManager::GetKeyboard() const
{
	return keyboard_;
}

const IMouse* DirectXInputManager::GetMouse() const
{
	return mouse_;
}