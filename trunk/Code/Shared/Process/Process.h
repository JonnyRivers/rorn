#pragma once

#include <string>

namespace Rorn
{
	namespace Process
	{
		void LaunchApplication(const char* path, const char* arguments);
		int RunCommandLineApplication(const char* path, const char* arguments, std::string& standardOutput);
	}
}