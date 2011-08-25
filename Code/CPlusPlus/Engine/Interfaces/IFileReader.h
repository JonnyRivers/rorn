#pragma once

#include "../../Shared/RornMaths/Float4.h"

namespace Rorn
{
	namespace Engine
	{
		struct IFileReader
		{
		public:
			virtual void ReadData(void* buffer, int length) = 0;
			virtual float ReadFloat() = 0;
			virtual void ReadFloat4(Rorn::Maths::Float4& float4) = 0;
			virtual int ReadInt() = 0;
			virtual unsigned int ReadUInt() = 0;
		};
	}
}