// PhysicsDemo001.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PhysicsDemo001.h"

#include "PhysicsDemo001App.h"

int APIENTRY _tWinMain(HINSTANCE instanceHandle,
                       HINSTANCE previousInstanceHandle,
                       LPTSTR commandLine,
                       int cmdShow)
{
	UNREFERENCED_PARAMETER(previousInstanceHandle);

	PhysicsDemo001App application;
	if (!application.InitInstance(instanceHandle, commandLine, cmdShow))
	{
		application.ExitInstance();
		return FALSE;
	}

	LoadAccelerators(instanceHandle, MAKEINTRESOURCE(IDC_PHYSICSDEMO001));

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
