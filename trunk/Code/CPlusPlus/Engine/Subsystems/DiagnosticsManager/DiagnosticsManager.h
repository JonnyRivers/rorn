#pragma once

#include <fstream>

#include <Windows.h>

namespace Rorn
{
	namespace Engine
	{
		// Unused for now...
		class DiagnosticsManager
		{
		public:
			static DiagnosticsManager& GetInstance();

			HRESULT Startup(HWND applicationWindowHandle);
			void Shutdown();

			std::ofstream& GetLoggingStream();
			void ReportError(LPCTSTR errorMessage) const;
			void ReportError(HRESULT hr, LPCTSTR errorHeading) const;
		private:
			static DiagnosticsManager& instance_;

			DiagnosticsManager(void);

			HWND applicationWindowHandle_;
			std::ofstream loggingStream_;
		};
	}
}