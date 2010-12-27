#include "Element.h"

using namespace Rorn::XML;

Element::Element(const char* name)
	: name_(name)
{
	
}

void Element::Serialize(Writer& writer) const
{
	writer.WriteText("<");
	writer.WriteText(name_.c_str());
	for(std::vector<ElementAttribute>::const_iterator it = attributes_.begin() ; it != attributes_.end() ; ++it)
	{
		writer.WriteText(" ");
		it->Serialize(writer);
	}
	writer.WriteText(">");

	SerializeDerived(writer);

	writer.WriteText("</");
	writer.WriteText(name_.c_str());
	writer.WriteText(">");
}
