#pragma once

#include "../../Engine/Interfaces/IEngine.h"

#include <windows.h>

class ModelViewerApp
{
public:
	ModelViewerApp(void);
	~ModelViewerApp(void);

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
	unsigned int modelId_;
	unsigned int modelInstanceId_;
};
