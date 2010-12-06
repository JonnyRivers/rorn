#pragma once

#include <vector>

#include "XMLElement.h"
#include "XMLValueElement.h"
#include "XMLWriter.h"

class XMLHierarchyElement : public XMLElement
{
public:
	XMLHierarchyElement(const char* name);
	virtual ~XMLHierarchyElement() {}

	XMLHierarchyElement& AddChildHierarchyElement(const char* name);
	XMLValueElement& AddChildValueElement(const char* name, const char* value);
protected:
	virtual void SerializeDerived(XMLWriter& writer) const;
private:
	std::vector<XMLHierarchyElement> childHierarchyElements_;
	std::vector<XMLValueElement> childValueElements_;
};

