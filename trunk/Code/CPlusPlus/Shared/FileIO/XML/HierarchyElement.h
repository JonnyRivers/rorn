#pragma once

#include <vector>

#include "Element.h"
#include "ValueElement.h"
#include "Writer.h"

namespace Rorn
{
	namespace XML
	{
		class HierarchyElement : public Element
		{
		public:
			HierarchyElement(const char* name);
			virtual ~HierarchyElement() {}

			HierarchyElement& AddChildHierarchyElement(const char* name);
			ValueElement& AddChildValueElement(const char* name, const char* value);
		protected:
			virtual void SerializeDerived(Writer& writer) const;
		private:
			std::vector<std::shared_ptr<Element>> childElements_;
		};
	}
}
