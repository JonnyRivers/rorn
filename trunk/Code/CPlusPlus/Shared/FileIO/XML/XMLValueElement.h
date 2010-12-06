#pragma once

#include <string>

#include "XMLElement.h"
#include "XMLWriter.h"

class XMLValueElement : public XMLElement
{
public:
	XMLValueElement(const char* name, const char* value);
	virtual ~XMLValueElement() {}
protected:
	virtual void SerializeDerived(XMLWriter& writer) const;
private:
	std::string name_;
	std::string value_;
};

