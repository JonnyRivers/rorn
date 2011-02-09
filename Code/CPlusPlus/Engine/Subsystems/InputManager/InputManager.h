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
			bool IsKeyDown(int key);
		private:
			static InputManager& instance_;

			InputManager(void);

			IDirectInput8* directInputSubsystem_;

			IDirectInputDevice8W* keyboardDevice_;
			unsigned char keyboardState_[256];
		};
	}
}