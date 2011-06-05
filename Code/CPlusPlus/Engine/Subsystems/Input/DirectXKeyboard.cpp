#pragma once

#include <cassert>

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "DirectXKeyboard.h"

using namespace Rorn::Engine;

DirectXKeyboard::DirectXKeyboard(HWND applicationWindowHandle, IDiagnostics* diagnostics, IDirectInput8* directInput) : diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "DirectXKeyboard instance is being created." << std::endl;

	HRESULT hr;
	if( FAILED( hr = directInput->CreateDevice(GUID_SysKeyboard, &device_, NULL) ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create input device (GUID_SysKeyboard).  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create input device (GUID_SysKeyboard).");
	}

	diagnostics_->GetLoggingStream() << "Input device (GUID_SysKeyboard) was created successfully." << std::endl;

	if( FAILED( hr = device_->SetDataFormat(&c_dfDIKeyboard) ) )
	{
		diagnostics->GetLoggingStream() << "Unable to set keyboard data format.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to set keyboard data format.");
	}

	diagnostics_->GetLoggingStream() << "Keyboard data format was set successfully." << std::endl;

	if( FAILED( hr = device_->SetCooperativeLevel(applicationWindowHandle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE) ) )
	{
		diagnostics->GetLoggingStream() << "Unable to set keyboard cooperative level.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to set keyboard cooperative level.");
	}

	diagnostics_->GetLoggingStream() << "The keyboard cooperative level was set successfully." << std::endl;

	if( FAILED( hr = device_->Acquire() ) )
	{
		diagnostics->GetLoggingStream() << "Unable to acquire keyboard.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to acquire keyboard.");
	}

	diagnostics_->GetLoggingStream() << "The keyboard was acquired." << std::endl;

	diagnostics_->GetLoggingStream() << "DirectXKeyboard instance was created successfully." << std::endl;
}

DirectXKeyboard::~DirectXKeyboard()
{
	diagnostics_->GetLoggingStream() << "DirectXKeyboard instance is being destroyed." << std::endl;

	if(device_ != NULL)
		device_->Release();

	diagnostics_->GetLoggingStream() << "DirectXKeyboard instance was destroyed successfully." << std::endl;
}

/*virtual*/ bool DirectXKeyboard::IsKeyDown(int key) const
{
	assert(key >= 0);
	assert(key <= 255);

	return ((deviceState_[key] & 0x80) == 0x80);
}