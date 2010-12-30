#include "FileManager.h"

using namespace Rorn::Engine;

/*static*/ FileManager& FileManager::instance_ = FileManager();// init static instance

/*static*/ FileManager& FileManager::GetInstance()
{
	return instance_;
}

FileManager::FileManager(void)
{
	
}
