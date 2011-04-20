#pragma once

#include "ci_string.h"

namespace Rorn
{
	namespace Text
	{
		namespace Path
		{
			Rorn::Text::ci_string GetDirectoryFromPathName(const Rorn::Text::ci_string& pathName);
			Rorn::Text::ci_string GetFileNameFromPathName(const Rorn::Text::ci_string& pathName);
			Rorn::Text::ci_string ChangeExtension(const Rorn::Text::ci_string& pathName, const Rorn::Text::ci_string& extension);
		}
	}
}