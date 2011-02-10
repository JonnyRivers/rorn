#pragma once

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace Rorn
{
	namespace Engine
	{
		class InputManager
		{
		public:
			static InputManager& GetInstance();

			HRESULT Startup(HWND windowHandle);
			void Shutdown();

			void Step();

			bool IsKeyDown(int key) const;

			bool IsLeftMouseButtonDown() const;
			bool IsRightMouseButtonDown() const;
			bool IsMouseWheelButtonDown() const;
			long GetMouseXMovement() const;
			long GetMouseYMovement() const;
			long GetMouseZMovement() const;
		private:
			static InputManager& instance_;

			InputManager(void);

			HRESULT StartupKeyboard(HWND windowHandle);
			void ShutdownKeyboard();
			HRESULT StartupMouse(HWND windowHandle);
			void ShutdownMouse();

			IDirectInput8* directInputSubsystem_;

			IDirectInputDevice8W* keyboardDevice_;
			unsigned char keyboardState_[256];

			IDirectInputDevice8W* mouseDevice_;
			DIMOUSESTATE2 mouseState_;
		};
	}
}