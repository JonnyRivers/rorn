#pragma once

#include <list>

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IFileSystem.h"
#include "../../Interfaces/IFileReader.h"
#include "../../Interfaces/IDiagnostics.h"

#include "PCFileReader.h"

namespace Rorn
{
	namespace Engine
	{
		class PCFileSystem : public IFileSystem
		{
		public:
			PCFileSystem(IDiagnostics* diagnostics);
			~PCFileSystem();

			IFileReader* CreateFileReader(const wchar_t* pathname);
			void DestroyFileReader(IFileReader* fileReader);
		private:
			PCFileSystem(PCFileSystem&);
			PCFileSystem& operator=(PCFileSystem&);

			IDiagnostics* diagnostics_;

			std::list<std::unique_ptr<PCFileReader>> openFileReaders_;
		};
	}
}