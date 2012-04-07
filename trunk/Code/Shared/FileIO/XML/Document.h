#pragma once

#include <string>

#include "HierarchyElement.h"

namespace Rorn
{
	namespace XML
	{
		class Document
		{
		public:
			~Document();

			static Document Load(const char* pathname);
			const HierarchyElement& GetRootElement();
		private:
			Document(const std::string& version, const HierarchyElement* rootElement);

			std::string version_;
			const HierarchyElement* rootElement_;
		};
	}
}
