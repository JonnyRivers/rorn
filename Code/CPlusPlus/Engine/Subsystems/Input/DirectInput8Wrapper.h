#pragma once

#include <windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class DirectInput8Wrapper
		{
		public:
			DirectInput8Wrapper(IDiagnostics* diagnostics);
			~DirectInput8Wrapper();

			operator IDirectInput8*();
		private:
			DirectInput8Wrapper(DirectInput8Wrapper&);
			DirectInput8Wrapper& operator=(DirectInput8Wrapper&);

			IDiagnostics* diagnostics_;
			IDirectInput8* directInput_;
		};
	}
}