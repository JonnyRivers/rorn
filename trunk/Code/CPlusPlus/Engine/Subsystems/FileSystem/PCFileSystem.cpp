#pragma once

#include "../../Exceptions/initialisation_exception.h"

#include "PCFileSystem.h"

using namespace Rorn::Engine;

PCFileSystem::PCFileSystem(IDiagnostics* diagnostics) : diagnostics_(diagnostics)
{

}

PCFileSystem::~PCFileSystem()
{
}

IFileReader* PCFileSystem::CreateFileReader(const wchar_t* pathname)
{
	PCFileReader* newFileReader = new PCFileReader(pathname);
	openFileReaders_.push_back( std::unique_ptr<PCFileReader>(newFileReader) );

	return newFileReader;
}

void PCFileSystem::DestroyFileReader(IFileReader* fileReader)
{
	std::list<std::unique_ptr<PCFileReader>>::iterator fileReaderIter;
	for(fileReaderIter = openFileReaders_.begin() ; fileReaderIter != openFileReaders_.end() ; )
	{
		if( fileReaderIter->get() == fileReader )
		{
			fileReaderIter = openFileReaders_.erase(fileReaderIter);
		}
		else
		{
			++fileReaderIter;
		}
	}
}
