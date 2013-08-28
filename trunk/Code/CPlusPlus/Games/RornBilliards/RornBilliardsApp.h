#pragma once

#include "../../Engine/Interfaces/IEngine.h"

#include <windows.h>

class RornBilliardsApp
{
public:
	RornBilliardsApp(void);
	~RornBilliardsApp(void);

	BOOL InitInstance(HINSTANCE instanceHandle, const wchar_t* commandLine, int cmdShow);
	VOID ExitInstance();
	VOID Step();
private:
	ATOM RegisterClass();
	static LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK About(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	static const int maxLoadString_ = 100;

	static HINSTANCE instanceHandle_;
	TCHAR title_[maxLoadString_];
	TCHAR windowClassName_[maxLoadString_];
	HWND windowHandle_;

	Rorn::Engine::IEngine* theEngine_;
	unsigned int cameraId_;
	unsigned int tableModelId_;
	unsigned int cueBallModelId_;
	unsigned int eightBallModelId_;
	unsigned int redBallModelId_;
	unsigned int yellowBallModelId_;
	unsigned int cueBallModelInstanceId_;
	bool readyToFire_;
};
