#pragma once

#include <string>
#include <vector>

#include <Max.h>

#include "..\..\..\Shared\FileIO\XML\XMLDocumentBuilder.h"

// This class simply satisfies the Max SDK requirement to derive from SceneExport.
// All of the real work is farmed out to SceneValidator and SceneExporter.
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
	int DoExport(const TCHAR* filename, ExpInterface* expInterface, Interface* maxInterface, BOOL suppressPrompts, DWORD options);
	BOOL SupportsOptions(int ext, DWORD options);
private:
	static void ExportScene(const TCHAR* filename, Interface* maxInterface);
	static void ExportNodeRecursive(INode* parentNode, XMLHierarchyElement& parentElement);
	static void ExportNode(INode* node, XMLHierarchyElement& nodeElement);
	static void ExportMaterial(Mtl* material, XMLHierarchyElement& parentElement);
	static void ExportMesh(Mesh& mesh, XMLHierarchyElement& meshElement);
	static void ExportPoint(const char* name, const Point3& point, XMLHierarchyElement& meshElement);
	static void ExportFace(const char* name, Face& face, XMLHierarchyElement& meshElement);
	
};

