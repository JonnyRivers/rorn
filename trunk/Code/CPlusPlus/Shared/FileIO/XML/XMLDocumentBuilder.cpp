#include "XMLDocumentBuilder.h"

#include "XMLWriter.h"

XMLDocumentBuilder::XMLDocumentBuilder(const char* rootElementName)
	: version_("1.0"), rootElement_(rootElementName)
{
}

XMLHierarchyElement& XMLDocumentBuilder::GetRootElement()
{
	return rootElement_;
}

void XMLDocumentBuilder::Save(const char* fileName) const
{
	XMLWriter writer(fileName);

	writer.WriteText("<?xml version=\"");
	writer.WriteText(version_.c_str());
	writer.WriteText("\" ?>");
	writer.WriteLine();

	rootElement_.Serialize(writer);
}