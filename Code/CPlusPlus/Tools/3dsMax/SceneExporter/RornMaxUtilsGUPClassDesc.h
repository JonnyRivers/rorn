#pragma once

// Max includes
#include <Max.h>

// Local includes
#include "RornMaxUtilsGUP.h"

class RornMaxUtilsGUPClassDesc : public ClassDesc 
{
public:
	RornMaxUtilsGUPClassDesc(void) {}
	~RornMaxUtilsGUPClassDesc(void) {}

	int IsPublic() { return 1; }
	void* Create(BOOL loading = FALSE) { return new RornMaxUtilsGUP(); }
	const TCHAR* ClassName() { return _T("Rorn Max Utils"); }
	SClass_ID SuperClassID() { return GUP_CLASS_ID; }
	Class_ID ClassID() { return Class_ID(0x666abcdd,0); }
	const TCHAR* Category() { return _T("Tools");  }
};

