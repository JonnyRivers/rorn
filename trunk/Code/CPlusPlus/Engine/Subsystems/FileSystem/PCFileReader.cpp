#include "PCFileReader.h"

#include <sstream>

#include "../../Exceptions/filesystem_exception.h"

using namespace Rorn::Engine;

PCFileReader::PCFileReader(const wchar_t* pathName)
{
	fileHandle_ = ::CreateFile(
		pathName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		NULL);

	if( fileHandle_ == INVALID_HANDLE_VALUE )
	{
		std::stringstream exceptionMessageStream;
		exceptionMessageStream << "Unable to open file for reading at '" << pathName << "'.";

		throw filesystem_exception(exceptionMessageStream.str().c_str());
	}
}

PCFileReader::~PCFileReader()
{
	CloseHandle(fileHandle_);
}

/*virtual*/ void PCFileReader::ReadData(void* buffer, int length)
{
	DWORD bytesRead;
	ReadFile(fileHandle_, buffer, length, &bytesRead, NULL);
}

/*virtual*/ float PCFileReader::ReadFloat()
{
	float value;
	ReadData(&value, sizeof(value));

	return value;
}

/*virtual*/ void PCFileReader::ReadFloat4(Rorn::Maths::Float4& float4)
{
	ReadData(&float4, sizeof(float4));
}

/*virtual*/ int PCFileReader::ReadInt()
{
	int value;
	ReadData(&value, sizeof(value));

	return value;
}

/*virtual*/ unsigned int PCFileReader::ReadUInt()
{
	unsigned int value;
	ReadData(&value, sizeof(value));

	return value;
}
