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
			HierarchyElement(const char* name, const std::vector<ElementAttribute>& attributes, const std::vector<std::shared_ptr<Element>> childElements);
			virtual ~HierarchyElement() {}

			HierarchyElement& AddChildHierarchyElement(const char* name);
			ValueElement& AddChildValueElement(const char* name, const char* value);

			const char* GetChildElementValue(const char* childElementName) const;
		protected:
			virtual void SerializeDerived(Writer& writer) const;
		private:
			std::vector<std::shared_ptr<Element>> childElements_;
		};
	}
}
