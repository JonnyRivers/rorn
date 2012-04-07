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

			const char* GetName() const;
			const char* GetValue() const;
		protected:
			virtual void SerializeDerived(Writer& writer) const;
		private:
			std::string value_;
		};
	}
}
