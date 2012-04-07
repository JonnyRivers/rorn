#include "WindowsRegistry.h"

HKEY Rorn::WindowsRegistry::GetHiveKeyFromEnum(RegistryHive registryHive)
{
	switch( registryHive )
	{
		case LocalMachine:
			return HKEY_LOCAL_MACHINE;
		default:
			return (HKEY)INVALID_HANDLE_VALUE;
	}
}

bool Rorn::WindowsRegistry::GetRegistryTextValue(RegistryHive registryHive, const char* subKeyName, const char* valueName, std::string& textValue)
{
	char buffer[260];
	DWORD bufferSize = sizeof(buffer);

	HKEY hiveKey = GetHiveKeyFromEnum(registryHive);
	HKEY subKey;
	LSTATUS openKeyStatus = ::RegOpenKey(
		HKEY_LOCAL_MACHINE,
		subKeyName,
		&subKey);

	if( openKeyStatus != ERROR_SUCCESS)
		return false;

	LSTATUS queryValueStatus = ::RegQueryValueEx(
		subKey,
		valueName,
		NULL,
		NULL,
		reinterpret_cast<BYTE*>(buffer),
		&bufferSize);

	if( queryValueStatus != ERROR_SUCCESS)
		return false;

	textValue = std::string(buffer);

	::RegCloseKey(subKey);

	return true;
}