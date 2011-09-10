#include "StdAfx.h"
#include "ModelViewerApp.h"

#include <sstream>

#include "../../Engine/PCEngine.h"
#include "../../Engine/Exceptions/model_load_exception.h"
#include "../../Engine/Exceptions/rorn_engine_exception.h"

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

	try
	{
		theEngine_ = PCEngine::Startup(windowHandle_);

		theEngine_->GetDiagnostics()->GetLoggingStream() << "The Model Viewer says: 'Hello world!'" << std::endl;

		modelId_ = theEngine_->GetRenderer()->LoadModel(commandLine);
		modelInstanceId_ = theEngine_->GetRenderer()->CreateModelInstance(modelId_, Matrix4x4::BuildIdentity());

		IModel* model = theEngine_->GetRenderer()->GetModel(modelId_);
		const Rorn::Maths::BoundingBox& modelBoundingBox = model->GetBoundingBox();
		
		cameraId_ = theEngine_->GetRenderer()->CreateFreeCamera(
			Vector3(0.0f, modelBoundingBox.GetMaximum().Y * 2.0f, modelBoundingBox.GetMinimum().Z * 4.0f),
			EulerAngles(0.0f, 0.35f, 0.0f));
		theEngine_->GetRenderer()->SetCurrentCamera(cameraId_);

		theEngine_->GetRenderer()->SetAmbientLight(
			Float4(0.05f, 0.05f, 0.05f, 1.0f));

		theEngine_->GetRenderer()->SetMainLight(
			Vector3(0.0f, -sin(Rorn::Maths::PiOverFour), sin(Rorn::Maths::PiOverFour)),
			Float4(0.0f, 0.0f, 0.0f, 1.0f));

		// TEMP - point lighting test
		unsigned int pointLight1Id = theEngine_->GetRenderer()->CreatePointLight(
			Vector3(-600.0f, 1270.0f, 0.0f),
			Float4(1.0f, 1.0f, 1.0f, 1.0f),
			60000000.0f);// distances are in mm, so luminosity is 60W * 1000^2
		unsigned int pointLight2Id = theEngine_->GetRenderer()->CreatePointLight(
			Vector3(600.0f, 1270.0f, 0.0f),
			Float4(1.0f, 1.0f, 1.0f, 1.0f),
			60000000.0f);// distances are is in mm, so luminosity is 60W * 1000^2
	}
	catch(Rorn::Engine::model_load_exception& ex)
	{
		::MessageBoxA(windowHandle_, ex.what(), "Unable to load model", MB_OK);
		return FALSE;
	}
	catch(Rorn::Engine::rorn_engine_exception& ex)
	{
		::MessageBoxA(windowHandle_, ex.what(), "Unable to create engine", MB_OK);
		return FALSE;
	}

	return TRUE;
}

VOID ModelViewerApp::ExitInstance()
{
	PCEngine::Shutdown(theEngine_);
}

VOID ModelViewerApp::Step()
{
	float timeElapsed = theEngine_->StartFrame();

	ICamera* camera = theEngine_->GetRenderer()->GetCamera(cameraId_);
	IModel* model = theEngine_->GetRenderer()->GetModel(modelId_);
	IModelInstance* modelInstance = theEngine_->GetRenderer()->GetModelInstance(modelInstanceId_);

	if( theEngine_->GetKeyboard()->IsKeyDown(DIK_COMMA) )
	{
		modelInstance->RotateY(timeElapsed);
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_PERIOD) )
	{
		modelInstance->RotateY(-timeElapsed);
	}

	//// TODO - refactor this out into some sort of 'camera controller'
	//// Move the camera
	//// base translation speed is quarter of the instance's bouding radius per second
	float translationSpeed = model->GetBoundingBox().GetBoundingRadius() * 0.25f;
	if( theEngine_->GetKeyboard()->IsKeyDown(DIK_LSHIFT) )
	{
		translationSpeed *= 5.0f;
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_LCONTROL) )
	{
		translationSpeed *= 0.2f;
	}

	Vector3 translation(0.0f, 0.0f, 0.0f);

	if( theEngine_->GetKeyboard()->IsKeyDown(DIK_D) )
	{
		translation.X += timeElapsed * translationSpeed;
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_A) )
	{
		translation.X -= timeElapsed * translationSpeed;
	}

	if( theEngine_->GetKeyboard()->IsKeyDown(DIK_E) )
	{
		translation.Y += timeElapsed * translationSpeed;
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_Q) )
	{
		translation.Y -= timeElapsed * translationSpeed;
	}

	if( theEngine_->GetKeyboard()->IsKeyDown(DIK_W) )
	{
		translation.Z += timeElapsed * translationSpeed;
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_S) )
	{
		translation.Z -= timeElapsed * translationSpeed;
	}

	camera->Translate(translation);

	if( theEngine_->GetMouse()->IsLeftButtonDown() )
	{
		if( theEngine_->GetMouse()->GetXMovement() != 0 )
		{
			float deltaHeading = 0.01f * static_cast<float>(theEngine_->GetMouse()->GetXMovement());
			camera->AlterHeading(deltaHeading);
		}

		if( theEngine_->GetMouse()->GetYMovement() != 0 )
		{
			float deltaPitch = 0.01f * static_cast<float>(theEngine_->GetMouse()->GetYMovement());
			camera->AlterPitch(deltaPitch);
		}
	}

	char fpsText[64];
	sprintf_s(fpsText, sizeof(fpsText), "FPS: %f", theEngine_->GetFramerate());
	theEngine_->GetRenderer()->AddDebugText(fpsText, -1.0f, 1.0f);

	theEngine_->EndFrame();
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