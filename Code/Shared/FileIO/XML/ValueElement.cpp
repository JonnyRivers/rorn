#include "ValueElement.h"

using namespace Rorn::XML;

ValueElement::ValueElement(const char* name, const char* value)
	: Element(name), value_(value)
{
}

/*virtual*/ void ValueElement::SerializeDerived(Writer& writer) const
{
	writer.WriteText(value_.c_str());
}

const char* ValueElement::GetName() const
{
	return name_.c_str();
}

const char* ValueElement::GetValue() const
{
	return value_.c_str();
}
