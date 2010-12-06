#include "XMLHierarchyElement.h"


XMLHierarchyElement::XMLHierarchyElement(const char* name) : XMLElement(name)
{
}

XMLHierarchyElement& XMLHierarchyElement::AddChildHierarchyElement(const char* name)
{
	XMLHierarchyElement* newElement = new XMLHierarchyElement(name);
	childElements_.push_back(std::shared_ptr<XMLElement>(newElement));

	return *newElement;
}

XMLValueElement& XMLHierarchyElement::AddChildValueElement(const char* name, const char* value)
{
	XMLValueElement* newElement = new XMLValueElement(name, value);
	childElements_.push_back(std::shared_ptr<XMLElement>(newElement));

	return *newElement;
}

/*virtual*/ void XMLHierarchyElement::SerializeDerived(XMLWriter& writer) const
{
	writer.IncreaseIndentLevel();

	for(std::vector<std::shared_ptr<XMLElement>>::const_iterator it = childElements_.begin() ; it != childElements_.end() ; ++it)
		(*it)->Serialize(writer);

	writer.DecreaseIndentLevel();
}
