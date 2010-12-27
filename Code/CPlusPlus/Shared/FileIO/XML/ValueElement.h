#pragma once

#include <string>

#include "Element.h"
#include "Writer.h"

namespace Rorn
{
	namespace XML
	{
		class ValueElement : public Element
		{
		public:
			ValueElement(const char* name, const char* value);
			virtual ~ValueElement() {}
		protected:
			virtual void SerializeDerived(Writer& writer) const;
		private:
			std::string name_;
			std::string value_;
		};
	}
}