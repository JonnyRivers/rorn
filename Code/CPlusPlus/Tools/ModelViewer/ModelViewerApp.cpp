#include "StdAfx.h"
#include "ModelViewerApp.h"

#include <fstream>

#include "../../Engine/Subsystems/DiagnosticsManager/DiagnosticsManager.h"
#include "../../Engine/Subsystems/RenderManager/RenderManager.h"
#include "../../Engine/Subsystems/RenderManager/ModelInstance.h"
#include "../../Engine/Subsystems/TimeManager/TimeManager.h"

#include "resource.h"

/*static*/ HINSTANCE ModelViewerApp::instanceHandle_ = NULL;

ModelViewerApp::ModelViewerApp(void)
{
	
}

ModelViewerApp::~ModelViewerApp(void)
{
}

BOOL ModelViewerApp::InitInstance(HINSTANCE instanceHandle, const wchar_t* commandLine, int cmdShow)
{
	instanceHandle_ = instanceHandle; // Store instance handle in our global variable
	::LoadString(instanceHandle_, IDS_APP_TITLE, title_, maxLoadString_);
	::LoadString(instanceHandle_, IDC_MODELVIEWER, windowClassName_, maxLoadString_);
	RegisterClass();
   
	// Create window
	RECT rc = { 0, 0, 1024, 768 };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, TRUE );
	windowHandle_ = CreateWindow(windowClassName_, title_, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instanceHandle_, NULL);

	if (!windowHandle_)
		return FALSE;

	ShowWindow(windowHandle_, cmdShow);
	UpdateWindow(windowHandle_);

	HRESULT hr = Rorn::Engine::DiagnosticsManager::GetInstance().Startup(windowHandle_);
	if( FAILED(hr) )
		return FALSE;

	hr = Rorn::Engine::RenderManager::GetInstance().Startup(windowHandle_);
	if( FAILED(hr) )
		return FALSE;

	// This MUST be done by the client.  So, should we make it part of the Startup()?
	XMVECTOR eye = XMVectorSet( 0.0f, 110.0f, -160.0f, 0.0f );
	XMVECTOR at = XMVectorSet( 0.0f, 40.0f, 0.0f, 0.0f );
	XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	camera_ = Rorn::Engine::RenderManager::GetInstance().CreateCamera(eye, at, up);
	Rorn::Engine::RenderManager::GetInstance().SetCurrentCamera(camera_);

	// This also MUST be done by the client.  So, should we make it part of the Startup()?
	XMFLOAT4 mainLightDirection(0, -.7071f, 0.7071f, 1.0f);
	XMFLOAT4 mainLightColor(0.5f, 0.5f, 0.5f, 1.0f);
	light_ = Rorn::Engine::RenderManager::GetInstance().CreateLight(mainLightDirection, mainLightColor);
	Rorn::Engine::RenderManager::GetInstance().SetMainLight(light_);

	// Setup ambient lighting
	XMFLOAT4 ambientLightColor(0.3f, 0.3f, 0.3f, 1.0f);
	Rorn::Engine::RenderManager::GetInstance().SetAmbientLightColor(ambientLightColor);

	model_ = Rorn::Engine::RenderManager::GetInstance().LoadOrGetModel(commandLine);

	XMMATRIX instanceToWorldMatrix = XMMatrixIdentity();
	modelInstance_ = Rorn::Engine::RenderManager::GetInstance().CreateModelInstance(model_, instanceToWorldMatrix);

	return TRUE;
}

VOID ModelViewerApp::ExitInstance()
{
	Rorn::Engine::RenderManager::GetInstance().Shutdown();
	Rorn::Engine::DiagnosticsManager::GetInstance().Shutdown();
}

VOID ModelViewerApp::Step()
{
	float secondsPassed;
	Rorn::Engine::TimeManager::GetInstance().Step(secondsPassed);
	modelInstance_->RotateY(secondsPassed);
	Rorn::Engine::RenderManager::GetInstance().Step();
}

ATOM ModelViewerApp::RegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= ModelViewerApp::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= instanceHandle_;
	wcex.hIcon			= LoadIcon(instanceHandle_, MAKEINTRESOURCE(IDI_MODELVIEWER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MODELVIEWER);
	wcex.lpszClassName	= windowClassName_;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK ModelViewerApp::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			::DialogBox(instanceHandle_, MAKEINTRESOURCE(IDD_ABOUTBOX), windowHandle, &ModelViewerApp::About);
			break;
		case IDM_EXIT:
			DestroyWindow(windowHandle);
			break;
		default:
			return DefWindowProc(windowHandle, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(windowHandle, &ps);
		EndPaint(windowHandle, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		{
			if( wParam == VK_F1 )
			{
				float fps = Rorn::Engine::TimeManager::GetInstance().GetFPS();
				Rorn::Engine::DiagnosticsManager::GetInstance().GetLoggingStream() << "FPS:" << fps << std::endl;
			}
			break;
		}

	default:
		return DefWindowProc(windowHandle, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK ModelViewerApp::About(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(windowHandle, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}