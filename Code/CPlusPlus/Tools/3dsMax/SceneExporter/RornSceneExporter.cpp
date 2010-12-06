#include "RornSceneExporter.h"

#include "SceneEnumerator.h"

#include <fstream>

RornSceneExporter::RornSceneExporter() 
{
}

RornSceneExporter::~RornSceneExporter()
{
}

int RornSceneExporter::ExtCount() 
{
	return 1;
}

const TCHAR* RornSceneExporter::Ext(int n) 
{		// Extensions supported for import/export modules
	switch(n) 
	{
		case 0:
			return _T(".Xml");
	}

	return _T("");
}

const TCHAR* RornSceneExporter::LongDesc() 
{			
	return _T("Rorn Engine Scene File");
}
	
const TCHAR* RornSceneExporter::ShortDesc() 
{			
	return _T("Rorn Scene");
}

const TCHAR* RornSceneExporter::AuthorName() 
{			
	return _T("Jonny Rivers");
}

const TCHAR* RornSceneExporter::CopyrightMessage() 
{	// ASCII Copyright message
	return _T("Copyright 2010 Riversoft");
}

const TCHAR* RornSceneExporter::OtherMessage1() 
{		// Other message #1
	return _T("");
}

const TCHAR* RornSceneExporter::OtherMessage2() 
{		// Other message #2
	return _T("");
}

unsigned int RornSceneExporter::Version() 
{
	return 1;
}

void RornSceneExporter::ShowAbout(HWND hWnd) 
{
}

int RornSceneExporter::DoExport(const TCHAR* filename, 
								ExpInterface* expInterface, 
								Interface* maxInterface, 
								BOOL suppressPrompts, 
								DWORD options) 
{
	SceneEnumerator sceneEnumerator;
	expInterface->theScene->EnumTree(&sceneEnumerator);
	sceneEnumerator.GetDocumentBuilder().Save(filename);

	return IMPEXP_SUCCESS;
}

BOOL RornSceneExporter::SupportsOptions(int ext, DWORD options) 
{
	assert(ext == 0);	// We only support one extension

	return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}
