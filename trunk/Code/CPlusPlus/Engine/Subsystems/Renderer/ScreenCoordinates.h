#pragma once

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class ScreenCoordinates
		{
		public:
			ScreenCoordinates(HWND windowHandle, IDiagnostics* diagnostics);

			unsigned int GetWidth() const;
			unsigned int GetHeight() const;
			unsigned int GetAspectRatio() const;
		private:
			unsigned int width_;
			unsigned int height_;
			float aspectRatio_;
		};
	}
}