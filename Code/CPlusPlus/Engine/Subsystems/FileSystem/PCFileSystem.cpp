#pragma once

#include "../../Exceptions/initialisation_exception.h"

#include "PCFileSystem.h"

#include "PCFileStream.h"

using namespace Rorn::Engine;

PCFileSystem::PCFileSystem(IDiagnostics* diagnostics) : diagnostics_(diagnostics)
{

}

PCFileSystem::~PCFileSystem()
{
}

/*virtual*/ StreamReader PCFileSystem::OpenRead(const wchar_t* pathname)
{
	return StreamReader(new PCFileStream(pathname));// The reader takes ownership of the stream
}

