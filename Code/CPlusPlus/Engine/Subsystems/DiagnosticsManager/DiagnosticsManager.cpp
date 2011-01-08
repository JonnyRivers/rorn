#include "DiagnosticsManager.h"

#include <sstream>

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

std::wofstream& DiagnosticsManager::GetLoggingStream()
{
	return loggingStream_;
}

void DiagnosticsManager::ReportError(const wchar_t* errorMessage)
{
	LogError(errorMessage);

	::MessageBox(applicationWindowHandle_, errorMessage, L"Error", MB_OK);
}

void DiagnosticsManager::ReportError(HRESULT hr, const wchar_t* errorHeading)
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

	std::wstringstream messageStream;
	messageStream << std::wstring(errorHeading) << std::endl 
				<< "Reason: " << formattedHRText;

	LogError(messageStream.str().c_str());

	if( formattedHRText != NULL )
	{
		::MessageBox(applicationWindowHandle_, messageStream.str().c_str(), L"Error", MB_OK);

		::LocalFree(formattedHRText);
	}	
}

void DiagnosticsManager::LogError(const wchar_t* errorMessage)
{
	loggingStream_ << "*** Error ***" << std::endl;
	loggingStream_ << errorMessage;
	loggingStream_ << "*************" << std::endl;
}