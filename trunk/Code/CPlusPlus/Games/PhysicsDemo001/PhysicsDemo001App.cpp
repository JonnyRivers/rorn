#include "StdAfx.h"
#include "PhysicsDemo001App.h"

#include <sstream>

#include "../../Engine/PCEngine.h"
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

/*static*/ HINSTANCE PhysicsDemo001App::instanceHandle_ = NULL;

PhysicsDemo001App::PhysicsDemo001App(void) : theEngine_(NULL), readyToFire_(true)
{
	
}

PhysicsDemo001App::~PhysicsDemo001App(void)
{
}

BOOL PhysicsDemo001App::InitInstance(HINSTANCE instanceHandle, const wchar_t* commandLine, int cmdShow)
{
	instanceHandle_ = instanceHandle; // Store instance handle in our global variable
	::LoadString(instanceHandle_, IDS_APP_TITLE, title_, maxLoadString_);
	::LoadString(instanceHandle_, IDC_PHYSICSDEMO001, windowClassName_, maxLoadString_);
	RegisterClass();
   
	// Create window
	RECT rc = { 0, 0, 1600, 900 };
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

		unsigned int groundModelId = theEngine_->GetRenderer()->LoadModel(_T("Ground001.model"));
		theEngine_->GetRenderer()->CreateModelInstance(groundModelId, Matrix4x4::BuildIdentity());

		unsigned int boxModelId = theEngine_->GetRenderer()->LoadModel(_T("Box001.model"));
		Matrix4x4 boxTransform = Matrix4x4::BuildIdentity();
		for(float x = -3 ; x <= 3 ; ++x)
		{
			for(float y = 40 ; y <= 46 ; ++y)
			{
				for(float z = -3 ; z <= 3 ; ++z)
				{
					boxTransform.M41 = x;
					boxTransform.M42 = y;
					boxTransform.M43 = z;
					theEngine_->GetRenderer()->CreateModelInstance(boxModelId, boxTransform);
				}
			}
		}

		projectileBoxModelId_ = theEngine_->GetRenderer()->LoadModel(_T("ProjectileBox001.model"));
		
		cameraId_ = theEngine_->GetRenderer()->CreateFreeCamera(
			Vector4(0.0f, 60.0f, -90.0f, 1.0f),
			EulerAngles(0.0f, 0.6f, 0.0f));
		theEngine_->GetRenderer()->SetCurrentCamera(cameraId_);

		theEngine_->GetRenderer()->SetAmbientLight(
			Float4(0.4f, 0.4f, 0.4f, 1.0f));

		theEngine_->GetRenderer()->SetMainLight(
			Vector4(0.0f, -sin(Rorn::Maths::PiOverFour), sin(Rorn::Maths::PiOverFour), 0.0f),
			Float4(0.4f, 0.4f, 0.4f, 1.0f));
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

VOID PhysicsDemo001App::ExitInstance()
{
	PCEngine::Shutdown(theEngine_);
}

VOID PhysicsDemo001App::Step()
{
	float timeElapsed = theEngine_->StartFrame();

	ICamera* camera = theEngine_->GetRenderer()->GetCamera(cameraId_);

	//if( theEngine_->GetKeyboard()->IsKeyDown(DIK_COMMA) )
	//{
	//	modelInstance->RotateY(timeElapsed);
	//}
	//else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_PERIOD) )
	//{
	//	modelInstance->RotateY(-timeElapsed);
	//}

	//// TODO - refactor this out into some sort of 'camera controller'
	//// Move the camera
	//// base translation speed is quarter of the instance's bouding radius per second
	float translationSpeed = 10.0f;
	if( theEngine_->GetKeyboard()->IsKeyDown(DIK_LSHIFT) )
	{
		translationSpeed *= 50.0f;
	}
	else if( theEngine_->GetKeyboard()->IsKeyDown(DIK_LCONTROL) )
	{
		translationSpeed *= 2.0f;
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

	if( readyToFire_ )
	{
		if( theEngine_->GetKeyboard()->IsKeyDown(DIK_SPACE) )
		{
			// fire a box
			Rorn::Maths::Vector4 cameraPosition;
			camera->GetPosition(cameraPosition);

			Rorn::Maths::Vector4 cameraEyeDirection;
			camera->GetEyeDirection(cameraEyeDirection);

			Rorn::Maths::Vector4 linearVelocity( cameraEyeDirection * 40.0f );

			Matrix4x4 projectileToWorldTransform = Matrix4x4::BuildIdentity();
			projectileToWorldTransform.M41 = cameraPosition.X;
			projectileToWorldTransform.M42 = cameraPosition.Y;
			projectileToWorldTransform.M43 = cameraPosition.Z;
			unsigned int projectileModelInstanceId = theEngine_->GetRenderer()->CreateModelInstance(projectileBoxModelId_, projectileToWorldTransform);
			IModelInstance* projectileModelInstance = theEngine_->GetRenderer()->GetModelInstance(projectileModelInstanceId);
			projectileModelInstance->SetLinearVelocity(linearVelocity);
			readyToFire_ = false;
		}
	}
	else
	{
		if( !theEngine_->GetKeyboard()->IsKeyDown(DIK_SPACE) )
		{
			readyToFire_ = true;
		}
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

ATOM PhysicsDemo001App::RegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= PhysicsDemo001App::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= instanceHandle_;
	wcex.hIcon			= LoadIcon(instanceHandle_, MAKEINTRESOURCE(IDI_PHYSICSDEMO001));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PHYSICSDEMO001);
	wcex.lpszClassName	= windowClassName_;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK PhysicsDemo001App::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
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
			::DialogBox(instanceHandle_, MAKEINTRESOURCE(IDD_ABOUTBOX), windowHandle, &PhysicsDemo001App::About);
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

INT_PTR CALLBACK PhysicsDemo001App::About(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
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