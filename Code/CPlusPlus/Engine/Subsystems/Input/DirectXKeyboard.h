#pragma once

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IDiagnostics.h"

#include "../../Interfaces/IKeyboard.h"

namespace Rorn
{
	namespace Engine
	{
		class DirectXKeyboard : public IKeyboard
		{
		public:
			DirectXKeyboard(HWND applicationWindowHandle, IDiagnostics* diagnostics, IDirectInput8* directInput);
			~DirectXKeyboard();

			virtual bool IsKeyDown(int key) const;
		private:
			DirectXKeyboard(DirectXKeyboard&);
			DirectXKeyboard& operator=(DirectXKeyboard&);

			IDiagnostics* diagnostics_;
			IDirectInputDevice8W* device_;
			unsigned char deviceState_[256];
		};
	}
}