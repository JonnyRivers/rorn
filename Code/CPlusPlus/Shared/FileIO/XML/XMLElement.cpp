#include "XMLElement.h"

XMLElement::XMLElement(const char* name)
	: name_(name)
{
	
}

void XMLElement::Serialize(XMLWriter& writer) const
{
	writer.WriteText("<");
	writer.WriteText(name_.c_str());
	for(std::vector<XMLElementAttribute>::const_iterator it = attributes_.begin() ; it != attributes_.end() ; ++it)
	{
		writer.WriteText(" ");
		it->Serialize(writer);
	}
	writer.WriteText(">");

	SerializeDerived(writer);

	writer.WriteText("</");
	writer.WriteText(name_.c_str());
	writer.WriteText(">");
	writer.WriteLine();
}
