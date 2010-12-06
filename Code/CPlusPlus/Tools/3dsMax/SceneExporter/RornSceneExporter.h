#pragma once

// Max includes
#include <Max.h>

class RornSceneExporter : public SceneExport
{
public:
	RornSceneExporter();
	~RornSceneExporter();

	int ExtCount();
	const TCHAR* Ext(int n);
	const TCHAR* LongDesc();
	const TCHAR* ShortDesc();
	const TCHAR* AuthorName();
	const TCHAR* CopyrightMessage();
	const TCHAR* OtherMessage1();
	const TCHAR* OtherMessage2();
	unsigned int Version();
	void ShowAbout(HWND hWnd);
	int DoExport(const TCHAR* name, ExpInterface* ei, Interface* i, BOOL suppressPrompts, DWORD options);
	BOOL SupportsOptions(int ext, DWORD options);
};

