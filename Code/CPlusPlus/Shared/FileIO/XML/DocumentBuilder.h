#pragma once

#include <string>

#include "HierarchyElement.h"

namespace Rorn
{
	namespace XML
	{
		class DocumentBuilder
		{
		public:
			DocumentBuilder(const char* rootElementName);

			HierarchyElement& GetRootElement();
			void Save(const char* fileName) const;
		private:
			std::string version_;
			HierarchyElement rootElement_;
		};
	}
}
