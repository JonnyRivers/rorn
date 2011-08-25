#pragma once

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IFileReader.h"

#include <Windows.h>

namespace Rorn
{
	namespace Engine
	{
		class PCFileReader : public IFileReader
		{
		public:
			PCFileReader(const wchar_t* pathname);
			~PCFileReader();

			void ReadData(void* buffer, int length);
			float ReadFloat();
			void ReadFloat4(Rorn::Maths::Float4& float4);
			int ReadInt();
			unsigned int ReadUInt();
		private:
			PCFileReader(PCFileReader&);
			PCFileReader& operator=(PCFileReader&);

			HANDLE fileHandle_;
		};
	}
}