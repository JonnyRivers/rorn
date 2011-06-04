#pragma once

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IDiagnostics.h"

#include "DirectXKeyboard.h"
#include "DirectXMouse.h"

namespace Rorn
{
	namespace Engine
	{
		class DirectXInputManager
		{
		public:
			DirectXInputManager(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~DirectXInputManager();

			const IKeyboard* GetKeyboard() const;
			const IMouse* GetMouse() const;
		private:
			DirectXInputManager(DirectXInputManager&);
			DirectXInputManager& operator=(DirectXInputManager&);

			IDiagnostics* diagnostics_;
			IDirectInput8* directInputSubsystem_;

			DirectXKeyboard* keyboard_;
			DirectXMouse* mouse_;
		};
	}
}