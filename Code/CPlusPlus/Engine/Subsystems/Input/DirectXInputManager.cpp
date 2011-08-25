#pragma once

#include "../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../Exceptions/initialisation_exception.h"

#include "DirectXInputManager.h"

using namespace Rorn::Engine;

DirectXInputManager::DirectXInputManager(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics),
	  directInput_(diagnostics),
	  keyboard_(applicationWindowHandle, diagnostics, directInput_), 
	  mouse_(applicationWindowHandle, diagnostics, directInput_)
{
	diagnostics_->GetLoggingStream() << "The DirectXInputManager subsystem was created successfully." << std::endl;
}

DirectXInputManager::~DirectXInputManager()
{
	diagnostics_->GetLoggingStream() << "The DirectXInputManager subsystem is being destroyed." << std::endl;
}

void DirectXInputManager::Step()
{
	keyboard_.Step();
	mouse_.Step();
}

const IKeyboard* DirectXInputManager::GetKeyboard() const
{
	return &keyboard_;
}

const IMouse* DirectXInputManager::GetMouse() const
{
	return &mouse_;
}