#pragma once

#include <fstream>

#include "ElementAttribute.h"
#include "HierarchyElement.h"

namespace Rorn
{
	namespace XML
	{
		class Reader
		{
		public:
			Reader(const char* fileName);
			~Reader();

			void ReadHeader(std::string& version);
			Element* ReadElement();
		private:
			void SkipWhitespace();
			void ReadElementAttributes(std::vector<ElementAttribute>& elementAttributes);

			const static int readBufferSize_ = 2048;

			std::ifstream ifs_;
			char readBuffer[readBufferSize_];
		};
	}
}