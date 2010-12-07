#pragma once

#include <set>

#include <Max.h>

#include "..\..\..\Shared\FileIO\XML\XMLDocumentBuilder.h"

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
private:
	static void ExportNodeRecursive(INode* parentNode, XMLHierarchyElement& parentElement);
	static void ExportNode(INode* node, XMLHierarchyElement& nodeElement);
	static void ExportMaterial(Mtl* material, XMLHierarchyElement& parentElement);
	static void ExportMesh(Mesh& mesh, XMLHierarchyElement& meshElement);
	static void ExportPoint(const char* name, const Point3& point, XMLHierarchyElement& meshElement);
	static void ExportFace(const char* name, Face& face, XMLHierarchyElement& meshElement);
	
};

