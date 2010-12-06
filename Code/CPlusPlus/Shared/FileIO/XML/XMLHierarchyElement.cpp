#include "XMLHierarchyElement.h"


XMLHierarchyElement::XMLHierarchyElement(const char* name) : XMLElement(name)
{
}

XMLHierarchyElement& XMLHierarchyElement::AddChildHierarchyElement(const char* name)
{
	childHierarchyElements_.push_back(XMLHierarchyElement(name));

	return childHierarchyElements_[childHierarchyElements_.size() - 1];
}

XMLValueElement& XMLHierarchyElement::AddChildValueElement(const char* name, const char* value)
{
	childValueElements_.push_back(XMLValueElement(name, value));

	return childValueElements_[childValueElements_.size() - 1];
}

/*virtual*/ void XMLHierarchyElement::SerializeDerived(XMLWriter& writer) const
{
	writer.IncreaseIndentLevel();

	for(std::vector<XMLHierarchyElement>::const_iterator it = childHierarchyElements_.begin() ; it != childHierarchyElements_.end() ; ++it)
		it->Serialize(writer);

	for(std::vector<XMLValueElement>::const_iterator it = childValueElements_.begin() ; it != childValueElements_.end() ; ++it)
		it->Serialize(writer);

	writer.DecreaseIndentLevel();
}
