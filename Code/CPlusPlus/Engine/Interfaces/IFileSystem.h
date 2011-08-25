#pragma once

#include "IFileReader.h"

namespace Rorn
{
	namespace Engine
	{
		struct IFileSystem
		{
		public:
			virtual IFileReader* CreateFileReader(const wchar_t* pathname) = 0;
			virtual void DestroyFileReader(IFileReader* fileReader) = 0;
		};
	}
}