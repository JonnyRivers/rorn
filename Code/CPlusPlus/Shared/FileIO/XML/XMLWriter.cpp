#include "XMLWriter.h"


XMLWriter::XMLWriter(const char* fileName)
	: ofs_(fileName), indentLevel_(0)
{
}

XMLWriter::~XMLWriter()
{
	ofs_.close();
}

void XMLWriter::WriteText(const char* text)
{
	ofs_ << text;
}

void XMLWriter::WriteLine()
{
	ofs_ << std::endl;
	for(unsigned int indent = 0 ; indent < indentLevel_ ; ++indent)
		ofs_ << '\t';
}

void XMLWriter::IncreaseIndentLevel()
{
	++indentLevel_;
}

void XMLWriter::DecreaseIndentLevel()
{
	--indentLevel_;
}