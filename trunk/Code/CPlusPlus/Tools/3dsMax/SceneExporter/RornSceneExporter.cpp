#include "RornSceneExporter.h"

#include "SceneExporter.h"
#include "SceneValidator.h"

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
{
	switch(n) 
	{
		case 0:
			return _T("Rorn.Scene.Xml");
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
{
	return _T("Copyright 2010 Riversoft");
}

const TCHAR* RornSceneExporter::OtherMessage1() 
{
	return _T("");
}

const TCHAR* RornSceneExporter::OtherMessage2() 
{
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
	SceneValidator sceneValidator(maxInterface);
	if(!sceneValidator.ValidateScene())
		return IMPEXP_FAIL;

	// Export the scene.  If this point is reached then it is safe to assume that all of the validation checks
	// have passed.  As a result, export code may seem careless but that is simply because error checking has been done
	// as part of the validation phase.
	SceneExporter sceneExporter(maxInterface);
	sceneExporter.ExportScene(std::string(filename));

	return IMPEXP_SUCCESS;
}

BOOL RornSceneExporter::SupportsOptions(int ext, DWORD options) 
{
	assert(ext == 0);

	return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}

