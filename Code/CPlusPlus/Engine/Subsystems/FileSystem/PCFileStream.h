#pragma once

#include <Windows.h>

#include "../../Interfaces/Stream.h"

namespace Rorn
{
	namespace Engine
	{
		class PCFileStream : public Stream
		{
		public:
			PCFileStream(const wchar_t* const pathname);
			~PCFileStream();

			void Close();

			void Read(void* data, unsigned int dataSize);
		private:
			PCFileStream(PCFileStream&);
			PCFileStream& operator=(PCFileStream&);

			HANDLE fileHandle_;
		};
	}
}
