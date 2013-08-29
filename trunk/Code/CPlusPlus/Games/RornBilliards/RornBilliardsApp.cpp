#include "StdAfx.h"
#include "RornBilliardsApp.h"

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

/*static*/ HINSTANCE RornBilliardsApp::instanceHandle_ = NULL;

RornBilliardsApp::RornBilliardsApp(void) : theEngine_(NULL), readyToFire_(true)
{
	
}

RornBilliardsApp::~RornBilliardsApp(void)
{
}

BOOL RornBilliardsApp::InitInstance(HINSTANCE instanceHandle, const wchar_t* commandLine, int cmdShow)
{
	instanceHandle_ = instanceHandle; // Store instance handle in our global variable
	::LoadString(instanceHandle_, IDS_APP_TITLE, title_, maxLoadString_);
	::LoadString(instanceHandle_, IDC_RORNBILLIARDS, windowClassName_, maxLoadString_);
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

		//theEngine_->GetPhysicsSystem()->SetEnabled(false);

		theEngine_->GetDiagnostics()->GetLoggingStream() << "Rorn Billiards says: 'Hello world!'" << std::endl;

		tableModelId_ = theEngine_->GetRenderer()->LoadModel(_T("EightFootPoolTable.model"));
		cueBallModelId_ = theEngine_->GetRenderer()->LoadModel(_T("CueBall.model"));
		eightBallModelId_ = theEngine_->GetRenderer()->LoadModel(_T("EightBall.model"));
		redBallModelId_ = theEngine_->GetRenderer()->LoadModel(_T("RedBall.model"));
		yellowBallModelId_ = theEngine_->GetRenderer()->LoadModel(_T("YellowBall.model"));

		const float tableSurfaceY = 0.9f;
		const float ballRadius = 0.0286f;
		const float ballDiameter = ballRadius * 2.0f;
		const float cueBallZOffset = -0.9f;
		const float eightBallZOffset = 0.9f;

		theEngine_->GetRenderer()->CreateModelInstance(tableModelId_, Matrix4x4::BuildIdentity());
		Matrix4x4 cueBallTransform(Matrix4x4::BuildIdentity());
		cueBallTransform.M41 = 0.0f;
		cueBallTransform.M42 = tableSurfaceY + ballRadius;
		cueBallTransform.M43 = cueBallZOffset;
		cueBallModelInstanceId_ = theEngine_->GetRenderer()->CreateModelInstance(cueBallModelId_, cueBallTransform);

		// six balls form a regular hexagon round the eightball (thus PiOverThree)
		//         15
		//       13  14
		//     10  11  12
		//   06  07  08  09
		// 01  02  03  04  05
		const float ballMargin = ballDiameter * 0.01f;
		const float eightBallX = 0.0f;
		const float eightBallZ = eightBallZOffset;
		const float moveLeftX = ballDiameter + ballMargin;
		const float moveLeftZ = 0;
		const float moveRightX = -moveLeftX;
		const float moveRightZ = 0;
		
		const float moveBackLeftX = (ballDiameter + ballMargin) * cos(Rorn::Maths::PiOverThree);
		const float moveBackLeftZ = (ballDiameter + ballMargin) * sin(Rorn::Maths::PiOverThree);
		const float moveForwardRightX = -moveBackLeftX;
		const float moveForwardRightZ = -moveBackLeftZ;

		float ballXPositions[15];
		float ballZPositions[15];
		int ballModelIds[15];

		ballXPositions[0] = eightBallX + moveLeftX + moveBackLeftX + moveBackLeftX;
		ballZPositions[0] = eightBallZ + moveLeftZ + moveBackLeftZ + moveBackLeftZ;
		ballModelIds[0] = redBallModelId_;
		ballXPositions[1] = ballXPositions[0] + moveRightX;
		ballZPositions[1] = ballZPositions[0] + moveRightZ;
		ballModelIds[1] = yellowBallModelId_;
		ballXPositions[2] = ballXPositions[1] + moveRightX;
		ballZPositions[2] = ballZPositions[1] + moveRightZ;
		ballModelIds[2] = yellowBallModelId_;
		ballXPositions[3] = ballXPositions[2] + moveRightX;
		ballZPositions[3] = ballZPositions[2] + moveRightZ;
		ballModelIds[3] = redBallModelId_;
		ballXPositions[4] = ballXPositions[3] + moveRightX;
		ballZPositions[4] = ballZPositions[3] + moveRightZ;
		ballModelIds[4] = yellowBallModelId_;

		ballXPositions[5] = ballXPositions[0] + moveForwardRightX;
		ballZPositions[5] = ballZPositions[0] + moveForwardRightZ;
		ballModelIds[5] = yellowBallModelId_;
		ballXPositions[6] = ballXPositions[5] + moveRightX;
		ballZPositions[6] = ballZPositions[5] + moveRightZ;
		ballModelIds[6] = redBallModelId_;
		ballXPositions[7] = ballXPositions[6] + moveRightX;
		ballZPositions[7] = ballZPositions[6] + moveRightZ;
		ballModelIds[7] = yellowBallModelId_;
		ballXPositions[8] = ballXPositions[7] + moveRightX;
		ballZPositions[8] = ballZPositions[7] + moveRightZ;
		ballModelIds[8] = redBallModelId_;

		ballXPositions[9] = ballXPositions[5] + moveForwardRightX;
		ballZPositions[9] = ballZPositions[5] + moveForwardRightZ;
		ballModelIds[9] = redBallModelId_;
		ballXPositions[10] = ballXPositions[9] + moveRightX;
		ballZPositions[10] = ballZPositions[9] + moveRightZ;
		ballModelIds[10] = eightBallModelId_;
		ballXPositions[11] = ballXPositions[10] + moveRightX;
		ballZPositions[11] = ballZPositions[10] + moveRightZ;
		ballModelIds[11] = yellowBallModelId_;

		ballXPositions[12] = ballXPositions[9] + moveForwardRightX;
		ballZPositions[12] = ballZPositions[9] + moveForwardRightZ;
		ballModelIds[12] = yellowBallModelId_;
		ballXPositions[13] = ballXPositions[12] + moveRightX;
		ballZPositions[13] = ballZPositions[12] + moveRightZ;
		ballModelIds[13] = redBallModelId_;

		ballXPositions[14] = ballXPositions[12] + moveForwardRightX;
		ballZPositions[14] = ballZPositions[12] + moveForwardRightZ;
		ballModelIds[14] = redBallModelId_;

		for(int i = 0 ; i < 15 ; ++i)
		{
			Matrix4x4 ballTransform(Matrix4x4::BuildIdentity());
			ballTransform.M41 = ballXPositions[i];
			ballTransform.M42 = tableSurfaceY + ballRadius;;
			ballTransform.M43 = ballZPositions[i];
			theEngine_->GetRenderer()->CreateModelInstance(ballModelIds[i], ballTransform);
		}
		
		cameraId_ = theEngine_->GetRenderer()->CreateFreeCamera(
			Vector4(0.0f, 4.0f, 0, 1.0f),
			EulerAngles(0.0f, Rorn::Maths::PiOverTwo, 0.0f));
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

VOID RornBilliardsApp::ExitInstance()
{
	PCEngine::Shutdown(theEngine_);
}

VOID RornBilliardsApp::Step()
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
			Rorn::Maths::Vector4 cameraUp;
			camera->GetUpVector(cameraUp);
			Rorn::Maths::Vector4 linearVelocity(cameraUp * 2.5f);
			linearVelocity.Y = 0;
			IModelInstance* cueBallModelInstance = theEngine_->GetRenderer()->GetModelInstance(cueBallModelInstanceId_);
			cueBallModelInstance->SetLinearVelocity(linearVelocity);
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

	char physicsText[64];
	int numActiveObjects = theEngine_->GetPhysicsSystem()->GetNumActiveObjects();
	sprintf_s(physicsText, sizeof(physicsText), "# active objects: %d", numActiveObjects);
	theEngine_->GetRenderer()->AddDebugText(physicsText, -1.0f, 0.95f);

	theEngine_->EndFrame();
}

ATOM RornBilliardsApp::RegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= RornBilliardsApp::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= instanceHandle_;
	wcex.hIcon			= LoadIcon(instanceHandle_, MAKEINTRESOURCE(IDI_RORNBILLIARDS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_RORNBILLIARDS);
	wcex.lpszClassName	= windowClassName_;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK RornBilliardsApp::WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
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
			::DialogBox(instanceHandle_, MAKEINTRESOURCE(IDD_ABOUTBOX), windowHandle, &RornBilliardsApp::About);
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

INT_PTR CALLBACK RornBilliardsApp::About(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
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