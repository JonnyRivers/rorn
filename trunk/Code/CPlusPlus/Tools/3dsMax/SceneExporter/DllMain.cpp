// Max includes
#include <Max.h>

// Local includes
#include "RornSceneExporter.h"
#include "RornSceneExporterClassDesc.h"

// Static data
static RornSceneExporterClassDesc theRornSceneExporterClassDesc;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, ULONG fdwReason, LPVOID lpvReserved) 
{
	switch(fdwReason) 
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hinstDLL);// Is this required?
			break;
		}
	}

	return TRUE;
}



__declspec( dllexport ) const TCHAR* LibDescription() 
{ 
	return _T("3DS File Exporter (Autodesk)"); 
}

__declspec( dllexport ) int LibNumberClasses() 
{ 
	return 1; 
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i) 
	{
		case 0:  
			return &theRornSceneExporterClassDesc; 
		default: 
			return 0; break;
	}
}

// Return version so can detect obsolete DLLs
__declspec( dllexport ) ULONG LibVersion() 
{ 
	return VERSION_3DSMAX; 
}

// Let the plug-in register itself for deferred loading
__declspec( dllexport ) ULONG CanAutoDefer()
{
	return 1;
}
