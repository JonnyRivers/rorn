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
		public:
			virtual ~Element() {}
			void Serialize(Writer& writer) const;
		protected:
			Element(const char* name);
			Element(const char* name, const std::vector<ElementAttribute>& attributes);
		
			virtual void SerializeDerived(Writer& writer) const = 0;

			std::string name_;
		private:
			std::vector<ElementAttribute> attributes_;
		};
	}
}

