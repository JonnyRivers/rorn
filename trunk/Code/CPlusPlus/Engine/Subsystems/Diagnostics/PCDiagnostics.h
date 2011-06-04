#pragma once

#include <fstream>

#include <Windows.h>

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class PCDiagnostics : public IDiagnostics
		{
		public:
			PCDiagnostics(HWND applicationWindowHandle);
			~PCDiagnostics();

			virtual std::wofstream& GetLoggingStream();
		private:
			PCDiagnostics(PCDiagnostics&);
			PCDiagnostics& operator=(PCDiagnostics&);

			HWND applicationWindowHandle_;
			std::wofstream loggingStream_;
		};
	}
}