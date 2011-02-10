#include "InputManager.h"

#include <cassert>

#include "../DiagnosticsManager/DiagnosticsManager.h"

using namespace Rorn::Engine;

/*static*/ InputManager& InputManager::instance_ = InputManager();// init static instance

/*static*/ InputManager& InputManager::GetInstance()
{
	return instance_;
}

InputManager::InputManager(void)
	: directInputSubsystem_(NULL), keyboardDevice_(NULL), mouseDevice_(NULL)
{
}

HRESULT InputManager::Startup(HWND windowHandle)
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Input Manager is starting up." << std::endl;

	HRESULT hr;
	if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
                                         IID_IDirectInput8, ( VOID** )&directInputSubsystem_, NULL ) ) )
    {
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of DirectInput subsystem");
		return hr;
	}

	if( FAILED( hr = StartupKeyboard(windowHandle) ) )
		return hr;

	if( FAILED( hr = StartupMouse(windowHandle) ) )
		return hr;

	std::wofstream& loggingStream = DiagnosticsManager::GetInstance().GetLoggingStream();
	loggingStream << "The DirectInput subsytem was successfully created." << std::endl;

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Input Manager started up successfully." << std::endl;

	return S_OK;
}

void InputManager::Shutdown()
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Input Manager is shutting down." << std::endl;

	ShutdownMouse();

	ShutdownKeyboard();

	if(directInputSubsystem_ != NULL)
		directInputSubsystem_->Release();

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Input Manager shut down successfully." << std::endl;
}

void InputManager::Step()
{
	HRESULT hr;
	if( FAILED( hr = keyboardDevice_->GetDeviceState(sizeof(keyboardState_), keyboardState_) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error getting the state of the keyboard device");
	}

	if( FAILED( hr = mouseDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error getting the state of the mouse device");
	}
}

bool InputManager::IsKeyDown(int key) const
{
	assert(key >= 0);
	assert(key <= 255);

	return ((keyboardState_[key] & 0x80) == 0x80);
}

bool InputManager::IsLeftMouseButtonDown() const
{
	return ((mouseState_.rgbButtons[0] & 0x80) == 0x80);
}

bool InputManager::IsRightMouseButtonDown() const
{
	return ((mouseState_.rgbButtons[1] & 0x80) == 0x80);
}

bool InputManager::IsMouseWheelButtonDown() const
{
	return ((mouseState_.rgbButtons[2] & 0x80) == 0x80);
}

long InputManager::GetMouseXMovement() const
{
	return mouseState_.lX;
}

long InputManager::GetMouseYMovement() const
{
	return mouseState_.lY;
}

long InputManager::GetMouseZMovement() const
{
	return mouseState_.lZ;
}

HRESULT InputManager::StartupKeyboard(HWND windowHandle)
{
	HRESULT hr;
	if( FAILED( hr = directInputSubsystem_->CreateDevice(GUID_SysKeyboard, &keyboardDevice_, NULL) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during keyboard device creation");
		return hr;
	}

	std::wofstream& loggingStream = DiagnosticsManager::GetInstance().GetLoggingStream();
	loggingStream << "The keyboard device was successfully created." << std::endl;

	if( FAILED( hr = keyboardDevice_->SetDataFormat(&c_dfDIKeyboard) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error while setting the keyboard data format");
		return hr;
	}

	loggingStream << "The keyboard data format was successfully set." << std::endl;

	if( FAILED( hr = keyboardDevice_->SetCooperativeLevel(windowHandle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error while setting the keyboard cooperative level");
		return hr;
	}

	loggingStream << "The keyboard cooperative level was successfully set." << std::endl;

	if( FAILED( hr = keyboardDevice_->Acquire() ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error while acquiring the keyboard device");
		return hr;
	}

	loggingStream << "The keyboard device was successfully acquired." << std::endl;

	return S_OK;
}

void InputManager::ShutdownKeyboard()
{
	if(keyboardDevice_ != NULL)
		keyboardDevice_->Release();
}

HRESULT InputManager::StartupMouse(HWND windowHandle)
{
	HRESULT hr;
	if( FAILED( hr = directInputSubsystem_->CreateDevice(GUID_SysMouse, &mouseDevice_, NULL) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during mouse device creation");
		return hr;
	}

	std::wofstream& loggingStream = DiagnosticsManager::GetInstance().GetLoggingStream();
	loggingStream << "The mouse device was successfully created." << std::endl;

	if( FAILED( hr = mouseDevice_->SetDataFormat(&c_dfDIMouse2) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error while setting the mouse data format");
		return hr;
	}

	loggingStream << "The mouse data format was successfully set." << std::endl;

	if( FAILED( hr = mouseDevice_->SetCooperativeLevel(windowHandle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error while setting the mouse cooperative level");
		return hr;
	}

	loggingStream << "The mouse cooperative level was successfully set." << std::endl;

	if( FAILED( hr = mouseDevice_->Acquire() ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error while acquiring the mouse device");
		return hr;
	}

	loggingStream << "The mouse device was successfully acquired." << std::endl;

	return S_OK;
}

void InputManager::ShutdownMouse()
{
	if(mouseDevice_ != NULL)
		mouseDevice_->Release();
}