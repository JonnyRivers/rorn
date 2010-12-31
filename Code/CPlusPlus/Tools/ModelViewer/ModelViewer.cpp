// ModelViewer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ModelViewer.h"

#include "../../Engine/Subsystems/RenderManager/RenderManager.h"
#include "../../Engine/Subsystems/TimeManager/TimeManager.h"

#include "ModelViewerApp.h"

int APIENTRY _tWinMain(HINSTANCE instanceHandle,
                       HINSTANCE previousInstanceHandle,
                       LPTSTR commandLine,
                       int cmdShow)
{
	UNREFERENCED_PARAMETER(previousInstanceHandle);
	UNREFERENCED_PARAMETER(commandLine);

	ModelViewerApp application;
	if (!application.InitInstance(instanceHandle, cmdShow))
	{
		application.ExitInstance();
		return FALSE;
	}

	LoadAccelerators(instanceHandle, MAKEINTRESOURCE(IDC_MODELVIEWER));

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
