#pragma once

#include <fstream>

namespace Rorn
{
	namespace XML
	{
		class Writer
		{
		public:
			Writer(const char* fileName);
			~Writer();

			void WriteText(const char* text);
			void WriteLine();
			void IncreaseIndentLevel();
			void DecreaseIndentLevel();
		private:
			std::ofstream ofs_;
			unsigned int indentLevel_;
		};
	}
}