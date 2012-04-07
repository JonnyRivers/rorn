#include <string>

#include <Windows.h>

namespace Rorn
{
	namespace WindowsRegistry
	{
		enum RegistryHive
		{
			LocalMachine,
		};

		HKEY GetHiveKeyFromEnum(RegistryHive registryHive);

		bool GetRegistryTextValue(RegistryHive registryHive, const char* subKeyName, const char* valueName, std::string& textValue);
	}
}