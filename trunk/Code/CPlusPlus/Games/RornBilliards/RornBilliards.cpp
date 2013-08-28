// RornBilliards.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RornBilliards.h"

#include "RornBilliardsApp.h"

int APIENTRY _tWinMain(HINSTANCE instanceHandle,
                       HINSTANCE previousInstanceHandle,
                       LPTSTR commandLine,
                       int cmdShow)
{
	UNREFERENCED_PARAMETER(previousInstanceHandle);

	RornBilliardsApp application;
	if (!application.InitInstance(instanceHandle, commandLine, cmdShow))
	{
		application.ExitInstance();
		return FALSE;
	}

	LoadAccelerators(instanceHandle, MAKEINTRESOURCE(IDC_RORNBILLIARDS));

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
