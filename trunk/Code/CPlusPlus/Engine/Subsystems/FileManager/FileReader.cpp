#include "FileReader.h"

using namespace Rorn::Engine;

FileReader::FileReader(LPCTSTR pathName)
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
	CloseHandle(fileHandle_);
}

void FileReader::ReadData(void* buffer, int length)
{
	DWORD bytesRead;
	ReadFile(fileHandle_, buffer, length, &bytesRead, NULL);
}

int FileReader::ReadInt()
{
	int value;
	ReadData(&value, sizeof(value));

	return value;
}

float FileReader::ReadFloat()
{
	float value;
	ReadData(&value, sizeof(value));

	return value;
}

