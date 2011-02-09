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
	: directInputSubsystem_(NULL), keyboardDevice_(NULL)
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

	std::wofstream& loggingStream = DiagnosticsManager::GetInstance().GetLoggingStream();
	loggingStream << "The DirectInput subsytem was successfully created." << std::endl;

	if( FAILED( hr = directInputSubsystem_->CreateDevice(GUID_SysKeyboard, &keyboardDevice_, NULL) ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during keyboard device creation");
		return hr;
	}

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

	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Input Manager started up successfully." << std::endl;

	return S_OK;
}

void InputManager::Shutdown()
{
	DiagnosticsManager::GetInstance().GetLoggingStream() << "The Input Manager is shutting down." << std::endl;

	if(keyboardDevice_ != NULL)
		keyboardDevice_->Release();

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
}

bool InputManager::IsKeyDown(int key)
{
	assert(key >= 0);
	assert(key <= 255);

	return ((keyboardState_[key] & 0x80) == 0x80);
}