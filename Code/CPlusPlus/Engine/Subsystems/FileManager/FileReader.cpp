#include "FileReader.h"

using namespace Rorn::Engine;

FileReader::FileReader(const char* pathName)
{
	fileHandle_ = ::CreateFileA(
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

int FileReader::ReadInt()
{
	int value;
	DWORD bytesRead;
	ReadFile(fileHandle_, &value, sizeof(value), &bytesRead, NULL);

	return value;
}

float FileReader::ReadFloat()
{
	float value;
	DWORD bytesRead;
	ReadFile(fileHandle_, &value, sizeof(value), &bytesRead, NULL);

	return value;
}

