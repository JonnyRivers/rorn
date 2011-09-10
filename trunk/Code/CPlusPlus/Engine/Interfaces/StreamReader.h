#pragma once

#include "../../Shared/RornMaths/Float4.h"

#include "Stream.h"

namespace Rorn
{
	namespace Engine
	{
		class StreamReader
		{
		public:
			StreamReader(Stream* stream);// The reader must take responsibility for the lifetime of the stream
			~StreamReader();

			StreamReader(StreamReader&);// Responsibility of stream ownership is transferred on copy
			StreamReader& operator=(StreamReader&);// Responsibility of stream ownership is transferred on copy

			void ReadData(void* buffer, int length);
			float ReadFloat();
			void ReadFloat4(Rorn::Maths::Float4& float4);
			int ReadInt();
			unsigned int ReadUInt();
		private:
			Stream* stream_;
		};
	}
}
