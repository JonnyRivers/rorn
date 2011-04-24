#include "FileReader.h"

using namespace Rorn::Engine;

FileReader::FileReader(const wchar_t* pathName)
{
	fileHandle_ = ::CreateFile(
		pathName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_READONLY,
		NULL);
}

FileReader::~FileReader()
{
	if( IsFileHandleValid() )
		CloseHandle(fileHandle_);
}

void FileReader::ReadData(void* buffer, int length)
{
	DWORD bytesRead;
	ReadFile(fileHandle_, buffer, length, &bytesRead, NULL);
}

float FileReader::ReadFloat()
{
	float value;
	ReadData(&value, sizeof(value));

	return value;
}

int FileReader::ReadInt()
{
	int value;
	ReadData(&value, sizeof(value));

	return value;
}

unsigned int FileReader::ReadUInt()
{
	unsigned int value;
	ReadData(&value, sizeof(value));

	return value;
}

bool FileReader::IsFileHandleValid() const
{
	return (fileHandle_ != INVALID_HANDLE_VALUE);
}