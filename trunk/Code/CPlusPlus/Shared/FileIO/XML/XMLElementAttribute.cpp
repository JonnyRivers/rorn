#include "XMLElementAttribute.h"

XMLElementAttribute::XMLElementAttribute(const char* name, const char* value)
	: name_(name), value_(value)
{
}

void XMLElementAttribute::Serialize(XMLWriter& writer) const
{
	writer.WriteText(name_.c_str());
	writer.WriteText("=\"");
	writer.WriteText(value_.c_str());
	writer.WriteText("\"");
}
