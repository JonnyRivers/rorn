#include "XMLValueElement.h"

XMLValueElement::XMLValueElement(const char* name, const char* value)
	: XMLElement(name), value_(value)
{
}

/*virtual*/ void XMLValueElement::SerializeDerived(XMLWriter& writer) const
{
	writer.WriteText(value_.c_str());
}