#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "XMLElementAttribute.h"
#include "XMLWriter.h"

class XMLElement
{
protected:
	XMLElement(const char* name);
	virtual ~XMLElement() {}
public:
	void Serialize(XMLWriter& writer) const;
protected:
	virtual void SerializeDerived(XMLWriter& writer) const = 0;
private:
	std::string name_;
	std::vector<XMLElementAttribute> attributes_;
};
