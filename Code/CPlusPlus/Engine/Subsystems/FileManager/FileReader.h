#pragma once

#include <windows.h>

namespace Rorn
{
	namespace Engine
	{
		class FileReader
		{
		public:
			FileReader(const char* pathName);
			~FileReader();

			int ReadInt();
			float ReadFloat();
		private:
			
			HANDLE fileHandle_;
		};
	}
}