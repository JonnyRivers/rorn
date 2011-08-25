#pragma once

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "../../Interfaces/IDiagnostics.h"

#include "../../Interfaces/IMouse.h"

namespace Rorn
{
	namespace Engine
	{
		class DirectXMouse : public IMouse
		{
		public:
			DirectXMouse(HWND applicationWindowHandle, IDiagnostics* diagnostics, IDirectInput8* directInput);
			~DirectXMouse();

			void Step();

			virtual bool IsLeftButtonDown() const;
			virtual bool IsRightButtonDown() const;
			virtual bool IsWheelButtonDown() const;
			virtual long GetXMovement() const;
			virtual long GetYMovement() const;
			virtual long GetZMovement() const;
		private:
			DirectXMouse(DirectXMouse&);
			DirectXMouse& operator=(DirectXMouse&);

			IDiagnostics* diagnostics_;
			IDirectInputDevice8W* device_;
			DIMOUSESTATE2 deviceState_;
		};
	}
}