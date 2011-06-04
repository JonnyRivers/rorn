#pragma once

#include <fstream>

namespace Rorn
{
	namespace Engine
	{
		struct IDiagnostics
		{
		public:
			virtual std::wofstream& GetLoggingStream() = 0;
		};
	}
}