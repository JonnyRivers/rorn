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

			std::wofstream& GetLoggingStream();
			void ReportError(const wchar_t* errorMessage);
			void ReportError(HRESULT hr, const wchar_t* errorHeading);
		private:
			static DiagnosticsManager& instance_;

			DiagnosticsManager(void);

			void LogError(const wchar_t* errorMessage);

			HWND applicationWindowHandle_;
			std::wofstream loggingStream_;
		};
	}
}