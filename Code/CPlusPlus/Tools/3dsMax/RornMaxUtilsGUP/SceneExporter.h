#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

#include <Max.h>
#include <stdmat.h>

#include "..\..\..\Shared\FileIO\XML\DocumentBuilder.h"
#include "..\..\..\Shared\Text\ci_string.h"

#include "TextureExporter.h"

class SceneExporter
{
public:
	SceneExporter(Interface* maxInterface);
	~SceneExporter(void);

	void ExportScene(const Rorn::Text::ci_string& filename);
private:
	void ExportNodeRecursive(INode* parentNode, Rorn::XML::HierarchyElement& parentElement);
	void ExportNode(INode* node, Rorn::XML::HierarchyElement& nodeElement);
	void ExportMatrix3(const char* name, const Matrix3& matrix, Rorn::XML::HierarchyElement& parentElement);
	void ExportMesh(INode* node, Mesh& mesh, Rorn::XML::HierarchyElement& meshElement);
	void ExportPoint3(const char* name, const Point3& point, Rorn::XML::HierarchyElement& parentElement);
	void ExportPhysics(INode* node, Rorn::XML::HierarchyElement& nodeElement);
	void ExportMaterials(Rorn::XML::HierarchyElement& parentElement);
	void ExportMaterial(Mtl* material, int id, Rorn::XML::HierarchyElement& materialElement);
	void ExportStandardMaterial(StdMat* material, Rorn::XML::HierarchyElement& materialElement);
	void ExportDiffuseBitmap(BitmapTex* diffuseBitmap, Rorn::XML::HierarchyElement& textureMapsElement);

	Interface* maxInterface_;
	Rorn::XML::DocumentBuilder xmlDocBuilder_;
	std::map<Mtl*, int> materials_;
	std::vector<Mtl*> orderedMaterials_;
	int numNodesExported_;
	int numMaterialsExported_;
	TextureExporter textureExporter_;
};

