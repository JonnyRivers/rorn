// XMLTestbed.cpp : Defines the entry point for the console application.
//

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include "../../FileIO/XML/Document.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Rorn::XML::Document configDocument = Rorn::XML::Document::Load("Z:/Config.xml");
	std::string projectsConfigPathname = configDocument.GetRootElement().GetChildElementValue("ProjectsConfigPathname");
	std::string toolchainDirectory = configDocument.GetRootElement().GetChildElementValue("ToolchainDirectory");

	return 0;
}

