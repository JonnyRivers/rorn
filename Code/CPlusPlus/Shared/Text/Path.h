#pragma once

#include <string>

namespace Rorn
{
	namespace Text
	{
		namespace Path
		{
			std::string GetDirectoryFromPathName(const std::string& pathName);
			std::string GetFileNameFromPathName(const std::string& pathName);
		}
	}
}