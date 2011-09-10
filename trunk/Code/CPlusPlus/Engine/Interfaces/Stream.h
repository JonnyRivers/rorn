#pragma once

namespace Rorn
{
	namespace Engine
	{
		class Stream
		{
		public:
			virtual ~Stream() {}
			virtual void Close() = 0;

			virtual void Read(void* data, unsigned int dataSize) = 0;
		};
	}
}
