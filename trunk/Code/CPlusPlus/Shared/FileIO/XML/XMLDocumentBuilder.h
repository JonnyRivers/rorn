#pragma once

#include <string>

#include "XMLHierarchyElement.h"

class XMLDocumentBuilder
{
public:
	XMLDocumentBuilder(const char* rootElementName);

	XMLHierarchyElement& GetRootElement();
	void Save(const char* fileName) const;
private:
	std::string version_;
	XMLHierarchyElement rootElement_;
};

