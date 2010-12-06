#pragma once

// Max includes
#include <Max.h>

// Local includes
#include "RornSceneExporter.h"

class RornSceneExporterClassDesc : public ClassDesc 
{
public:
	RornSceneExporterClassDesc(void) {}
	~RornSceneExporterClassDesc(void) {}

	int IsPublic() { return 1; }
	void* Create(BOOL loading = FALSE) { return new RornSceneExporter(); }
	const TCHAR* ClassName() { return _T("Rorn Scene Exporter"); }
	SClass_ID SuperClassID() { return SCENE_EXPORT_CLASS_ID; }
	Class_ID ClassID() { return Class_ID(0x666abcde,0); }
	const TCHAR* Category() { return _T("Scene Export");  }
};

