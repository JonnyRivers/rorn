#include "PCDiagnostics.h"

#include <sstream>

#include <Windows.h>
#include <ShlObj.h>

using namespace Rorn::Engine;

PCDiagnostics::PCDiagnostics(HWND applicationWindowHandle)
{
	applicationWindowHandle_ = applicationWindowHandle;

	wchar_t appDataDirectory[MAX_PATH];
	HRESULT hr = ::SHGetFolderPathW(applicationWindowHandle_, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, appDataDirectory);
	if( FAILED( hr ) )
		throw initialisation_exception("Unable to retrieve application data directory");

	std::wstringstream logFilePathnameStream;
	logFilePathnameStream << appDataDirectory << "\\Rorn\\EngineLog.txt";

	loggingStream_.open(logFilePathnameStream.str().c_str());
	if(!loggingStream_.is_open())
		throw initialisation_exception("Unable to open diagnostics log file");
	
	loggingStream_ << "The Diagnostics subsystem started up successfully." << std::endl;
}

PCDiagnostics::~PCDiagnostics()
{
	if(loggingStream_.is_open())
	{
		loggingStream_ << "The Diagnostics subsystem began shutting down.  No further logging is possible." << std::endl;
		loggingStream_.close();
	}
}

/*virtual*/ std::wofstream& PCDiagnostics::GetLoggingStream()
{
	return loggingStream_;
}
