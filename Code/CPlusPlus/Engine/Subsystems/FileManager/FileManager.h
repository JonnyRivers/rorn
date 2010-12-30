#pragma once

#include <Windows.h>

namespace Rorn
{
	namespace Engine
	{
		// Unused for now...
		class FileManager
		{
		public:
			static FileManager& GetInstance();
		private:
			static FileManager& instance_;

			FileManager(void);
		};
	}
}