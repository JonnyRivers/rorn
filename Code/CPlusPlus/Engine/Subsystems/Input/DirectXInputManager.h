#pragma once

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "../../Interfaces/IDiagnostics.h"

#include "DirectInput8Wrapper.h"
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

			void Step();

			const IKeyboard* GetKeyboard() const;
			const IMouse* GetMouse() const;
		private:
			DirectXInputManager(DirectXInputManager&);
			DirectXInputManager& operator=(DirectXInputManager&);

			IDiagnostics* diagnostics_;
			DirectInput8Wrapper directInput_;
			DirectXKeyboard keyboard_;
			DirectXMouse mouse_;
		};
	}
}