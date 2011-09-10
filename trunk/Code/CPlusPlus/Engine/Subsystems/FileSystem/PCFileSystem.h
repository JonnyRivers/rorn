#pragma once

#include <list>

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IFileSystem.h"
#include "../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class PCFileSystem : public IFileSystem
		{
		public:
			PCFileSystem(IDiagnostics* diagnostics);
			~PCFileSystem();

			virtual StreamReader OpenRead(const wchar_t* pathname);
		private:
			PCFileSystem(PCFileSystem&);
			PCFileSystem& operator=(PCFileSystem&);

			IDiagnostics* diagnostics_;
		};
	}
}