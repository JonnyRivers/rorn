#include "Reader.h"

using namespace Rorn::XML;

namespace
{
	const char* versionPrefix_ = "version=";
}

Reader::Reader(const char* fileName)
	: ifs_(fileName)
{
}

Reader::~Reader()
{
	ifs_.close();
}

void Reader::ReadHeader(std::string& version)
{
	ifs_.getline(readBuffer, readBufferSize_);
	std::string header(readBuffer);
	std::string::size_type versionPrefixPos = header.find(versionPrefix_);
	std::string::size_type versionStartQuotePos = header.find('"', versionPrefixPos);
	std::string::size_type versionValuePos = versionStartQuotePos + 1;
	std::string::size_type versionEndQuotePos = header.find('"', versionValuePos);
	version = header.substr(versionValuePos, versionEndQuotePos - versionValuePos);
}

Element* Reader::ReadElement()
{
	SkipWhitespace();

	// read the opening tag (including attributes)
	std::ifstream::int_type tagStart = ifs_.get();
	std::string elementName;
	while(ifs_.peek() != '>' && ifs_.peek() != '/' && ifs_.peek() != ' ' && ifs_.peek() != '\t')
		elementName += ifs_.get();
	std::vector<ElementAttribute> attributes;

	if(ifs_.peek() == ' ' || ifs_.peek() == '\t')
	{
		ReadElementAttributes(attributes);
		SkipWhitespace();
	}

	// read a child element, an inner value or a closing tag (empty element)
	std::vector<std::shared_ptr<Element>> childElements;
	if(ifs_.peek() == '/')// empty element
	{
		ifs_.get();// skip '/'
		ifs_.get();// skip '>'

		return new ValueElement(elementName.c_str(), "");
	}
	else
	{
		ifs_.get();// skip '>'

		SkipWhitespace();
		if(ifs_.peek() == '<')// child element or end tag
		{
			bool endTagRead = false;
			while(!endTagRead)
			{
				ifs_.seekg(1, std::ios::cur);
				if( ifs_.peek() == '/' )
				{
					ifs_.get();
					while(ifs_.peek() != '>')
						ifs_.get();
					ifs_.get();
					endTagRead = true;
				}
				else
				{
					ifs_.seekg(-1, std::ios::cur);
					childElements.push_back(std::shared_ptr<Element>( ReadElement() ) );
					SkipWhitespace();
				}
			}
		}
		else// read the inner value up to the next '<' then skip the end tag
		{
			std::string innerText;
			while(ifs_.peek() != '<')
				innerText += ifs_.get();

			// skip the end tag
			ifs_.get();
			ifs_.get();
			while(ifs_.peek() != '>')
				ifs_.get();
			ifs_.get();

			return new ValueElement(elementName.c_str(), innerText.c_str());
		}
	}

	return new HierarchyElement(elementName.c_str(), attributes, childElements);
}

void Reader::SkipWhitespace()
{
	std::ifstream::int_type nextCharacter = ifs_.peek();
	while(nextCharacter == '\t' || nextCharacter == ' ' || nextCharacter == '\r' || nextCharacter == '\n')
	{
		ifs_.get();
		nextCharacter = ifs_.peek();
	}
}

void Reader::ReadElementAttributes(std::vector<ElementAttribute>& elementAttributes)
{

}
