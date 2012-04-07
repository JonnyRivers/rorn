#include "Writer.h"

using namespace Rorn::XML;

Writer::Writer(const char* fileName)
	: ofs_(fileName), indentLevel_(0)
{
}

Writer::~Writer()
{
	ofs_.close();
}

void Writer::WriteText(const char* text)
{
	ofs_ << text;
}

void Writer::WriteLine()
{
	ofs_ << std::endl;
	for(unsigned int indent = 0 ; indent < indentLevel_ ; ++indent)
		ofs_ << '\t';
}

void Writer::IncreaseIndentLevel()
{
	++indentLevel_;
}

void Writer::DecreaseIndentLevel()
{
	--indentLevel_;
}