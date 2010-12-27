#include "DocumentBuilder.h"

#include "Writer.h"

using namespace Rorn::XML;

DocumentBuilder::DocumentBuilder(const char* rootElementName)
	: version_("1.0"), rootElement_(rootElementName)
{
}

HierarchyElement& DocumentBuilder::GetRootElement()
{
	return rootElement_;
}

void DocumentBuilder::Save(const char* fileName) const
{
	Writer writer(fileName);

	writer.WriteText("<?xml version=\"");
	writer.WriteText(version_.c_str());
	writer.WriteText("\" ?>");
	writer.WriteLine();

	rootElement_.Serialize(writer);
}