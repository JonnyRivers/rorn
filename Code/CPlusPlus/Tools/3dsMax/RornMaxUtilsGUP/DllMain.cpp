// Max includes
#include <Max.h>

// Local includes
#include "RornMaxUtilsGUP.h"
#include "RornMaxUtilsGUPClassDesc.h"

// Static data
static RornMaxUtilsGUPClassDesc theRornMaxUtilsClassDesc;

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
	return _T("Rorn Engine Max Utilities"); 
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
			return &theRornMaxUtilsClassDesc; 
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
	return 0;
}
