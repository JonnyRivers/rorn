#include "ElementAttribute.h"

using namespace Rorn::XML;

ElementAttribute::ElementAttribute(const char* name, const char* value)
	: name_(name), value_(value)
{
}

void ElementAttribute::Serialize(Writer& writer) const
{
	writer.WriteText(name_.c_str());
	writer.WriteText("=\"");
	writer.WriteText(value_.c_str());
	writer.WriteText("\"");
}
