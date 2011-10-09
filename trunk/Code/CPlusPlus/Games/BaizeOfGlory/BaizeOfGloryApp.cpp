#include "StdAfx.h"
#include "BaizeOfGloryApp.h"

#include <sstream>

#include "../../Engine/PCEngine.h"
#include "../../Engine/Interfaces/IModelInstance.h"
#include "../../Engine/Exceptions/model_load_exception.h"
#include "../../Engine/Exceptions/rorn_engine_exception.h"

#include "../../Shared/RornMaths/Constants.h"
#include "../../Shared/RornMaths/BoundingBox.h"
#include "../../Shared/RornMaths/Float4.h"
#include "../../Shared/RornMaths/Matrix4x4.h"
#include "../../Shared/RornMaths/Vector4.h"

#include "resource.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;
using namespace Rorn::Physics;

/*static*/ HINSTANCE BaizeOfGloryApp::instanceHandle_ = NULL;

BaizeOfGloryApp::BaizeOfGloryApp(void) : theEngine_(NULL)
{
	
}

BaizeOfGloryApp::~BaizeOfGloryApp(void)
{
}

BOOL BaizeOfGloryApp::InitInstance(HINSTANCE instanceHandle, const wchar_t* /*commandLine*/, int cmdShow)
{
	instanceHandle_ = instanceHandle; // Store instance handle in our global variable
	::LoadString(instanceHandle_, IDS_APP_TITLE, title_, maxLoadString_);
	::LoadString(instanceHandle_, IDC_BAIZEOFGLORY, windowClassName_, maxLoadString_);
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

		theEngine_->GetDiagnostics()->GetLoggingStream() << "Baize fo Glory starting up" << std::endl;

		poolCueBallModelId_ = theEngine_->GetRenderer()->LoadModel(L"PoolCueBall.model");
		poolCueBallInstanceId_ = theEngine_->GetRenderer()->CreateModelInstance(poolCueBallModelId_, Matrix4x4::BuildIdentity());
		float tensorValue = 1.0f / (0.4f * 0.156f * (0.0238f * 0.0238f));// 2/5 * m * r^2
		cueBallPhysicsObject_ = RigidBody(
			Vector4(0.0f, 0.0f, 0.0f, 0.0f),
			Quaternion::CreateFromAxisAngle(Vector4(0.0f, 1.0f, 0.0f, 0.0f), 0.0f),
			0.156f,
			Matrix4x4(tensorValue,        0.0f,        0.0f, 0.0f,
					         0.0f, tensorValue,        0.0f, 0.0f,
					         0.0f,        0.0f, tensorValue, 0.0f,
					         0.0f,        0.0f,        0.0f, 0.0f));

		IModel* model = theEngine_->GetRenderer()->GetModel(poolCueBallModelId_);
		const Rorn::Maths::BoundingBox& modelBoundingBox = model->GetBoundingBox();
		
		cameraId_ = theEngine_->GetRenderer()->CreateFreeCamera(
			Vector4(0.0f, modelBoundingBox.GetMaximum().Y * 2.0f, modelBoundingBox.GetMinimum().Z * 16.0f, 1.0f),
			EulerAngles(0.0f, 0.35f, 0.0f));
		theEngine_->GetRenderer()->SetCurrentCamera(cameraId_);

		theEngine_->GetRenderer()->SetAmbientLight(
			Float4(0.2f, 0.2f, 0.2f, 1.0f));

		theEngine_->GetRenderer()->SetMainLight(
			Vector4(0.0f, -sin(Rorn::Maths::PiOverFour), sin(Rorn::Maths::PiOverFour), 0.0f),
			Float4(0.7f, 0.7f, 0.7f, 1.0f));
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

VOID BaizeOfGloryApp::ExitInstance()
{
	PCEngine::Shutdown(theEngine_);
}

VOID BaizeOfGloryApp::Step()
{
	float timeElapsed = theEngine_->StartFrame();

	ICamera* camera = theEngine_->GetRenderer()->GetCamera(cameraId_);
	IModel* model = theEngine_->GetRenderer()->GetModel(poolCueBallModelId_);
	IModelInstance* modelInstance = theEngine_->GetRenderer()->GetModelInstance(poolCueBallInstanceId_);

	if( theEngine_->GetKeyboard()->IsKeyDown(DIK_NUMPAD5) )// dead centre
	{
		cueBallPhysicsObject_.AddForce(
			Vector4(0.0f, 0.0f, 0.05f, 0.0f),
			Vector4(0.0f, 0.0f, -0.0238f, 0.0f));
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_NUMPAD8) )// Wee bit topspin
	{
		cueBallPhysicsObject_.AddForce(
			Vector4(0.0f, 0.0f, 0.05f, 0.0f),
			Vector4(0.0f, -0.016829f, -0.016829f, 0.0f));// mmmm, this should be +ve :-S
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_NUMPAD2) )// Wee bit bottomspin
	{
		cueBallPhysicsObject_.AddForce(
			Vector4(0.0f, 0.0f, 0.05f, 0.0f),
			Vector4(0.0f, 0.016829f, -0.016829f, 0.0f));// mmmm, this should be +ve :-S
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_NUMPAD4) )// Wee bit topspin
	{
		cueBallPhysicsObject_.AddForce(
			Vector4(0.0f, 0.0f, 0.05f, 0.0f),
			Vector4(0.016829f, 0.0f, -0.016829f, 0.0f));// mmmm, this should be +ve :-S
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_NUMPAD6) )// Wee bit bottomspin
	{
		cueBallPhysicsObject_.AddForce(
			Vector4(0.0f, 0.0f, 0.05f, 0.0f),
			Vector4(-0.016829f, 0.0f, -0.016829f, 0.0f));// mmmm, this should be +ve :-S
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_NUMPAD0) )// dead centre
	{
		cueBallPhysicsObject_.AddForce(
			Vector4(0.0f, 0.0f, -0.05f, 0.0f),
			Vector4(0.0f, 0.0f, 0.0238f, 0.0f));
	}

	cueBallPhysicsObject_.Step(timeElapsed);

	modelInstance->SetInstanceToWorldMatrix(cueBallPhysicsObject_.GetInstanceToWorldMatrix());

	//// TODO - refactor this out into some sort of 'camera controller'
	//// Move the camera
	//// base translation speed is quarter of the instance's bouding radius per second
	float translationSpeed = model->GetBoundingBox().GetBoundingRadius() * 10.0f;
	if( theEngine_->GetKeyboard()->IsKeyDown(DIK_LSHIFT) )
	{
		translationSpeed *= 10.0f;
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_LCONTROL) )
	{
		translationSpeed *= 0.2f;
	}

	Vector4 translation(0.0f, 0.0f, 0.0f, 0.0f);

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

ATOM BaizeOfGloryApp::RegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= BaizeOfGloryApp::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= instanceHandle_;
	wcex.hIcon			= LoadIcon(instanceHandle_, MAKEINTRESOURCE(IDI_BAIZEOFGLORY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_BAIZEOFGLORY);
	wcex.lpszClassName	= windowClassName_;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK BaizeOfGloryApp::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
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
			::DialogBox(instanceHandle_, MAKEINTRESOURCE(IDD_ABOUTBOX), windowHandle, &BaizeOfGloryApp::About);
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

INT_PTR CALLBACK BaizeOfGloryApp::About(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
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