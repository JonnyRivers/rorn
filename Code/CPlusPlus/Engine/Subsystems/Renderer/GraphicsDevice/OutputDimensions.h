#pragma once

#include <windows.h>

namespace Rorn
{
	namespace Engine
	{
		class OutputDimensions
		{
		public:
			OutputDimensions(HWND applicationWindowHandle);

			unsigned int Width;
			unsigned int Height;
			float AspectRatio;
		};
	}
}

