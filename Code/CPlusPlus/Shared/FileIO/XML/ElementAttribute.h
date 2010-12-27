#pragma once

#include <fstream>
#include <string>

#include "Writer.h"

namespace Rorn
{
	namespace XML
	{
		class ElementAttribute
		{
		public:
			ElementAttribute(const char* name, const char* value);

			void Serialize(Writer& writer) const;
		private:
			std::string name_;
			std::string value_;
		};
	}
}