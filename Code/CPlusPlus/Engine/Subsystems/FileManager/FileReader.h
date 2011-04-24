#pragma once

#include <windows.h>

namespace Rorn
{
	namespace Engine
	{
		class FileReader
		{
		public:
			FileReader(const wchar_t* pathName);
			~FileReader();

			void ReadData(void* buffer, int length);
			float ReadFloat();
			int ReadInt();
			unsigned int ReadUInt();

			bool IsFileHandleValid() const;
		private:
			
			HANDLE fileHandle_;
		};
	}
}