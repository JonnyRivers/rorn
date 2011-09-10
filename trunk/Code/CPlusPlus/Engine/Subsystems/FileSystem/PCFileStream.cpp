#include "PCFileStream.h"

#include <sstream>

#include "../../Exceptions/filesystem_exception.h"

using namespace Rorn::Engine;

PCFileStream::PCFileStream(const wchar_t* const pathname)
{
	fileHandle_ = ::CreateFile(
		pathname,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		NULL);

	if( fileHandle_ == INVALID_HANDLE_VALUE )
	{
		std::stringstream exceptionMessageStream;
		exceptionMessageStream << "Unable to open file for reading at '" << pathname << "'.";

		throw filesystem_exception(exceptionMessageStream.str().c_str());
	}
}

PCFileStream::~PCFileStream()
{
	Close();
}

void PCFileStream::Close()
{
	if( fileHandle_ != INVALID_HANDLE_VALUE )
	{
		::CloseHandle(fileHandle_);
		fileHandle_ = INVALID_HANDLE_VALUE;
	}
}

void PCFileStream::Read(void* data, unsigned int dataSize)
{
	// TODO - handle failure
	DWORD bytesRead;
	::ReadFile(fileHandle_, data, dataSize, &bytesRead, NULL);
}