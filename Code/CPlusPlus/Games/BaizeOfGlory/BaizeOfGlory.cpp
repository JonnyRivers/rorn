// BaizeOfGlory.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BaizeOfGlory.h"

#include "BaizeOfGloryApp.h"

int APIENTRY _tWinMain(HINSTANCE instanceHandle,
                       HINSTANCE previousInstanceHandle,
                       LPTSTR commandLine,
                       int cmdShow)
{
	UNREFERENCED_PARAMETER(previousInstanceHandle);

	if( wcslen(commandLine) == 0 )
	{
		::MessageBox(NULL, L"You must specify a valid model file path on the command line when running the Model Viewer", L"Invalid command line arguments", MB_OK);
		return FALSE;
	}

	BaizeOfGloryApp application;
	if (!application.InitInstance(instanceHandle, commandLine, cmdShow))
	{
		application.ExitInstance();
		return FALSE;
	}

	LoadAccelerators(instanceHandle, MAKEINTRESOURCE(IDC_BAIZEOFGLORY));

	// Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            application.Step();
        }
    }

	application.ExitInstance();

	return (int) msg.wParam;
}
