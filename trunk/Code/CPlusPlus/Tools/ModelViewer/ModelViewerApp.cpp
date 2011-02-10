#include "StdAfx.h"
#include "ModelViewerApp.h"

#include "../../Engine/Subsystems/DiagnosticsManager/DiagnosticsManager.h"
#include "../../Engine/Subsystems/InputManager/InputManager.h"
#include "../../Engine/Subsystems/RenderManager/RenderManager.h"
#include "../../Engine/Subsystems/RenderManager/FreeCamera.h"
#include "../../Engine/Subsystems/RenderManager/Model.h"
#include "../../Engine/Subsystems/RenderManager/ModelInstance.h"
#include "../../Engine/Subsystems/TextureManager/TextureManager.h"
#include "../../Engine/Subsystems/TimeManager/TimeManager.h"

#include "../../Shared/RornMaths/Constants.h"
#include "../../Shared/RornMaths/BoundingBox.h"
#include "../../Shared/RornMaths/Float4.h"
#include "../../Shared/RornMaths/Matrix4x4.h"
#include "../../Shared/RornMaths/Vector3.h"

#include "resource.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

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

	HRESULT hr = DiagnosticsManager::GetInstance().Startup(windowHandle_);
	if( FAILED(hr) )
		return FALSE;

	hr = InputManager::GetInstance().Startup(windowHandle_);
	if( FAILED(hr) )
		return FALSE;

	hr = RenderManager::GetInstance().Startup(windowHandle_);
	if( FAILED(hr) )
		return FALSE;

	model_ = RenderManager::GetInstance().LoadOrGetModel(commandLine);
	const Rorn::Maths::BoundingBox& modelBoundingBox = model_->GetBoundingBox();

	Matrix4x4 instanceToWorldMatrix = Matrix4x4::BuildIdentity();
	modelInstance_ = RenderManager::GetInstance().CreateModelInstance(model_, instanceToWorldMatrix);

	// This MUST be done by the client.  So, should we make it part of the Startup()?
	camera_ = RenderManager::GetInstance().CreateFreeCamera(
		Vector3(0.0f, modelBoundingBox.GetMaximum().Y * 0.5f, modelBoundingBox.GetMinimum().Z * 4.0f),// position
		EulerAngles(0.0f, 0.0f, 0.0f));
	RenderManager::GetInstance().SetCurrentCamera(camera_);

	// This also MUST be done by the client.  So, should we make it part of the Startup()?
	Vector3 mainLightDirection(0, -sin(Rorn::Maths::PiOver4), sin(Rorn::Maths::PiOver4));
	Float4 mainLightColor(1.0f, 1.0f, 1.0f, 1.0f);
	light_ = RenderManager::GetInstance().CreateLight(mainLightDirection, mainLightColor);
	RenderManager::GetInstance().SetMainLight(light_);

	// Setup ambient lighting
	Float4 ambientLightColor(0.2f, 0.2f, 0.2f, 1.0f);
	RenderManager::GetInstance().SetAmbientLightColor(ambientLightColor);

	return TRUE;
}

VOID ModelViewerApp::ExitInstance()
{
	RenderManager::GetInstance().Shutdown();
	InputManager::GetInstance().Shutdown();
	DiagnosticsManager::GetInstance().Shutdown();
}

VOID ModelViewerApp::Step()
{
	float secondsPassed;
	TimeManager::GetInstance().Step(secondsPassed);

	InputManager::GetInstance().Step();

	// Rotate the model in its local Y axis
	if( InputManager::GetInstance().IsKeyDown(DIK_COMMA) )
		modelInstance_->RotateY(secondsPassed);
	else if( InputManager::GetInstance().IsKeyDown(DIK_PERIOD) )
		modelInstance_->RotateY(-secondsPassed);

	// TODO - refactor this out into some sort of 'camera controller'
	// Move the camera
	float translationSpeed = 10.0f;// distance per second
	if(InputManager::GetInstance().IsKeyDown(DIK_LSHIFT))
		translationSpeed = 50.0f;
	else if(InputManager::GetInstance().IsKeyDown(DIK_LCONTROL))
		translationSpeed = 2.0f;

	Rorn::Maths::Vector3 translation(0.0f, 0.0f, 0.0f);
	if( InputManager::GetInstance().IsKeyDown(DIK_D) )
		translation.X += secondsPassed * translationSpeed;
	else if( InputManager::GetInstance().IsKeyDown(DIK_A) )
		translation.X -= secondsPassed * translationSpeed;
	if( InputManager::GetInstance().IsKeyDown(DIK_E) )
		translation.Y += secondsPassed * translationSpeed;
	else if( InputManager::GetInstance().IsKeyDown(DIK_Q) )
		translation.Y -= secondsPassed * translationSpeed;
	if( InputManager::GetInstance().IsKeyDown(DIK_W) )
		translation.Z += secondsPassed * translationSpeed;
	else if( InputManager::GetInstance().IsKeyDown(DIK_S) )
		translation.Z -= secondsPassed * translationSpeed;
	camera_->Translate(translation);

	if( InputManager::GetInstance().IsLeftMouseButtonDown() )
	{
		if( InputManager::GetInstance().GetMouseXMovement() != 0 )
		{
			float headingDelta = -0.01f * static_cast<float>(InputManager::GetInstance().GetMouseXMovement());
			camera_->AlterHeading(headingDelta);
		}

		if( InputManager::GetInstance().GetMouseYMovement() != 0 )
		{
			float pitchDelta = -0.01f * static_cast<float>(InputManager::GetInstance().GetMouseYMovement());
			camera_->AlterPitch(pitchDelta);
		}
	}

	RenderManager::GetInstance().Step();
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