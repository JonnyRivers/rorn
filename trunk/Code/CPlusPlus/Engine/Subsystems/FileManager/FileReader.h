#pragma once

#include <windows.h>

namespace Rorn
{
	namespace Engine
	{
		class FileReader
		{
		public:
			FileReader(LPCTSTR pathName);
			~FileReader();

			void ReadData(void* buffer, int length);
			int ReadInt();
			float ReadFloat();
		private:
			
			HANDLE fileHandle_;
		};
	}
}