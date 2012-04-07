#include "Document.h"

#include "Reader.h"

using namespace Rorn::XML;

/*static*/ Document Document::Load(const char* pathname)
{
	Reader reader(pathname);

	std::string version;
	reader.ReadHeader(version);
	HierarchyElement* rootElement = dynamic_cast<HierarchyElement*>(reader.ReadElement());

	return Document(version, rootElement);
}

Document::Document(const std::string& version, const HierarchyElement* rootElement) : version_(version), rootElement_(rootElement)
{
}

Document::~Document()
{
	delete rootElement_;
}

const HierarchyElement& Document::GetRootElement()
{
	return *rootElement_;
}