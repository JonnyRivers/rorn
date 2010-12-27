#include "HierarchyElement.h"

using namespace Rorn::XML;

HierarchyElement::HierarchyElement(const char* name) : Element(name)
{
}

HierarchyElement& HierarchyElement::AddChildHierarchyElement(const char* name)
{
	HierarchyElement* newElement = new HierarchyElement(name);
	childElements_.push_back(std::shared_ptr<Element>(newElement));

	return *newElement;
}

ValueElement& HierarchyElement::AddChildValueElement(const char* name, const char* value)
{
	ValueElement* newElement = new ValueElement(name, value);
	childElements_.push_back(std::shared_ptr<Element>(newElement));

	return *newElement;
}

/*virtual*/ void HierarchyElement::SerializeDerived(Writer& writer) const
{
	writer.IncreaseIndentLevel();
	writer.WriteLine();

	for(std::vector<std::shared_ptr<Element>>::const_iterator it = childElements_.begin() ; it != childElements_.end() ; ++it)
	{
		(*it)->Serialize(writer);
		if(std::distance(it, childElements_.end()) > 1)
			writer.WriteLine();// we need to decrease the indent level on the last one, so no new line
	}

	writer.DecreaseIndentLevel();
	writer.WriteLine();
}
