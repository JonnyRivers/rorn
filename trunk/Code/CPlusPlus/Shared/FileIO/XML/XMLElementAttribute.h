#pragma once

#include <fstream>
#include <string>

#include "XMLWriter.h"

class XMLElementAttribute
{
public:
	XMLElementAttribute(const char* name, const char* value);

	void Serialize(XMLWriter& writer) const;
private:
	std::string name_;
	std::string value_;
};

