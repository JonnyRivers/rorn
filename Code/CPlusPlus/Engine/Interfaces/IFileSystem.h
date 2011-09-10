#pragma once

#include "StreamReader.h"

namespace Rorn
{
	namespace Engine
	{
		struct IFileSystem
		{
		public:
			virtual StreamReader OpenRead(const wchar_t* pathname) = 0;
		};
	}
}