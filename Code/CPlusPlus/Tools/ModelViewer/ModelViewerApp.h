#pragma once

#include "../../Engine/Subsystems/RenderManager/RenderManager.h"

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

	Rorn::Engine::LookAtCamera* camera_;
	Rorn::Engine::Light* light_;
	Rorn::Engine::Model* model_;
	Rorn::Engine::ModelInstance* modelInstance_;
};
