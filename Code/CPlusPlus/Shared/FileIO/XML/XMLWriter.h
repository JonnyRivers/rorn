#pragma once

#include <fstream>

class XMLWriter
{
public:
	XMLWriter(const char* fileName);
	~XMLWriter();

	void WriteText(const char* text);
	void WriteLine();
	void IncreaseIndentLevel();
	void DecreaseIndentLevel();
private:
	std::ofstream ofs_;
	unsigned int indentLevel_;
};

