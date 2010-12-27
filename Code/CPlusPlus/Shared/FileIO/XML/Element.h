#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "ElementAttribute.h"
#include "Writer.h"

namespace Rorn
{
	namespace XML
	{
		class Element
		{
		protected:
			Element(const char* name);
			virtual ~Element() {}
		public:
			void Serialize(Writer& writer) const;
		protected:
			virtual void SerializeDerived(Writer& writer) const = 0;
		private:
			std::string name_;
			std::vector<ElementAttribute> attributes_;
		};
	}
}
