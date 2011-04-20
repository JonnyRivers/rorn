#include "Process.h"

#include <sstream>

#include <Windows.h>

#include "../Text/Path.h"

void Rorn::Process::LaunchApplication(const char* path, const char* arguments)
{
	HINSTANCE modelViewerInstanceHandle = ::ShellExecute(NULL, "open", path, arguments, Rorn::Text::Path::GetDirectoryFromPathName(path).c_str(), SW_SHOWNORMAL);
}

int Rorn::Process::RunCommandLineApplication(const char* path, const char* arguments, std::string& standardOutput)
{
    char tempPath[MAX_PATH];
    ::GetTempPath(sizeof(tempPath), tempPath);
    char tempFileName[MAX_PATH];
    ::GetTempFileName(tempPath, "rorn", 0, tempFileName);
    SECURITY_ATTRIBUTES securityAttributes = { 0 };
    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.bInheritHandle = TRUE;
    HANDLE standardOutputHandle = ::CreateFile(tempFileName, GENERIC_WRITE, FILE_SHARE_WRITE, &securityAttributes, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
 
    STARTUPINFO startupInfo;
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    startupInfo.wShowWindow = SW_HIDE;
    startupInfo.hStdOutput = standardOutputHandle;
 
    PROCESS_INFORMATION processInfo;
 
    if( !::CreateProcess(
        path,
        const_cast<char*>(arguments),
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &startupInfo,
        &processInfo) )
    {
        std::stringstream errorStream;
        errorStream << "Unable to create process.  GetLastError() returned " << ::GetLastError();
        standardOutput = errorStream.str();
 
        ::CloseHandle( standardOutputHandle );
        ::DeleteFile( tempPath );
        return 1;
    }
 
    ::WaitForSingleObject(processInfo.hProcess, INFINITE);
 
    DWORD exitCode = 0;
   
    ::GetExitCodeProcess(processInfo.hProcess, &exitCode);
 
    ::CloseHandle( processInfo.hProcess );
    ::CloseHandle( processInfo.hThread );
    ::CloseHandle( standardOutputHandle );
 

    // read it from the temp file
    standardOutputHandle = ::CreateFile(tempFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    char standardOutputBuffer[16 * 1024];
    DWORD bytesRead = 0;
    ::ReadFile(standardOutputHandle, standardOutputBuffer, sizeof(standardOutputBuffer), &bytesRead, NULL);
    standardOutputBuffer[bytesRead] = '\0';
       
    standardOutput = standardOutputBuffer;
 
    ::DeleteFile( tempFileName );
   
    return exitCode;
}