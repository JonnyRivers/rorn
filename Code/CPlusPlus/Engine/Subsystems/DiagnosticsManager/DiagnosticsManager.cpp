#include "DiagnosticsManager.h"

#include <tchar.h>

using namespace Rorn::Engine;

/*static*/ DiagnosticsManager& DiagnosticsManager::instance_ = DiagnosticsManager();// init static instance

/*static*/ DiagnosticsManager& DiagnosticsManager::GetInstance()
{
	return instance_;
}

DiagnosticsManager::DiagnosticsManager(void)
{
	
}

HRESULT DiagnosticsManager::Startup(HWND applicationWindowHandle)
{
	applicationWindowHandle_ = applicationWindowHandle;
	loggingStream_.open("RornEngineLog.txt");

	if(!loggingStream_.is_open())
		return E_ACCESSDENIED;
	
	loggingStream_ << "The Diagnostics Manager started up successfully." << std::endl;
	return S_OK;
}

void DiagnosticsManager::Shutdown()
{
	if(loggingStream_.is_open())
	{
		loggingStream_ << "The Diagnostics Manager began shutting down.  No further logging is possible." << std::endl;
		loggingStream_.close();
	}

	applicationWindowHandle_ = NULL;
}

std::ofstream& DiagnosticsManager::GetLoggingStream()
{
	return loggingStream_;
}

void DiagnosticsManager::ReportError(LPCTSTR errorMessage) const
{
	::MessageBox(applicationWindowHandle_, errorMessage, _T("Error"), MB_OK);
}

void DiagnosticsManager::ReportError(HRESULT hr, LPCTSTR errorHeading) const
{
	LPTSTR formattedHRText = NULL;

	::FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&formattedHRText),
		0,
		NULL);

	if( formattedHRText != NULL )
	{
		::MessageBox(applicationWindowHandle_, formattedHRText, errorHeading, MB_OK);

		::LocalFree(formattedHRText);
	}	
}