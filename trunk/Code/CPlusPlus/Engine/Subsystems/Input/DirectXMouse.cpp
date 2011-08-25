#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"
#include "../../Exceptions/input_exception.h"

#include "DirectXMouse.h"

using namespace Rorn::Engine;

DirectXMouse::DirectXMouse(HWND applicationWindowHandle, IDiagnostics* diagnostics, IDirectInput8* directInput) : diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "DirectXMouse instance is being created." << std::endl;

	HRESULT hr;
	if( FAILED( hr = directInput->CreateDevice(GUID_SysMouse, &device_, NULL) ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create input device (GUID_SysMouse).  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to create input device (GUID_SysMouse).");
	}

	diagnostics_->GetLoggingStream() << "Input device (GUID_SysMouse) was created successfully." << std::endl;

	if( FAILED( hr = device_->SetDataFormat(&c_dfDIMouse2) ) )
	{
		diagnostics->GetLoggingStream() << "Unable to set mouse data format.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to set mouse data format.");
	}

	diagnostics_->GetLoggingStream() << "Mouse data format was set successfully." << std::endl;

	if( FAILED( hr = device_->SetCooperativeLevel(applicationWindowHandle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE) ) )
	{
		diagnostics->GetLoggingStream() << "Unable to set mouse cooperative level.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to set mouse cooperative level.");
	}

	diagnostics_->GetLoggingStream() << "The mouse cooperative level was set successfully." << std::endl;

	if( FAILED( hr = device_->Acquire() ) )
	{
		diagnostics->GetLoggingStream() << "Unable to acquire mouse.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
		throw initialisation_exception("Unable to acquire mouse.");
	}

	diagnostics_->GetLoggingStream() << "The mouse was acquired." << std::endl;

	diagnostics_->GetLoggingStream() << "DirectXMouse instance was created successfully." << std::endl;
}

DirectXMouse::~DirectXMouse()
{
	diagnostics_->GetLoggingStream() << "DirectXMouse instance is being destroyed." << std::endl;

	if(device_ != NULL)
		device_->Release();

	diagnostics_->GetLoggingStream() << "DirectXMouse instance was destroyed successfully." << std::endl;
}

void DirectXMouse::Step()
{
	HRESULT hr;
	if( FAILED( hr = device_->GetDeviceState(sizeof(deviceState_), &deviceState_) ) )
	{
		throw input_exception("Unable to update mouse device state");
	}
}

/*virtual*/ bool DirectXMouse::IsLeftButtonDown() const
{
	return ((deviceState_.rgbButtons[0] & 0x80) == 0x80);
}

/*virtual*/ bool DirectXMouse::IsRightButtonDown() const
{
	return ((deviceState_.rgbButtons[1] & 0x80) == 0x80);
}

/*virtual*/ bool DirectXMouse::IsWheelButtonDown() const
{
	return ((deviceState_.rgbButtons[2] & 0x80) == 0x80);
}

/*virtual*/ long DirectXMouse::GetXMovement() const
{
	return deviceState_.lX;
}

/*virtual*/ long DirectXMouse::GetYMovement() const
{
	return deviceState_.lY;
}

/*virtual*/ long DirectXMouse::GetZMovement() const
{
	return deviceState_.lZ;
}