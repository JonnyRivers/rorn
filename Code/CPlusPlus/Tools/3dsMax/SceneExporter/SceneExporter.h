#pragma once

#include <map>
#include <string>
#include <vector>

#include <Max.h>
#include <stdmat.h>

#include "..\..\..\Shared\FileIO\XML\DocumentBuilder.h"

class SceneExporter
{
public:
	SceneExporter(Interface* maxInterface);
	~SceneExporter(void);

	void ExportScene(const std::string& filename);
private:
	void ExportNodeRecursive(INode* parentNode, Rorn::XML::HierarchyElement& parentElement);
	void ExportNode(INode* node, Rorn::XML::HierarchyElement& nodeElement);
	void ExportMesh(INode* node, Mesh& mesh, Rorn::XML::HierarchyElement& meshElement);
	void ExportPoint3(const char* name, const Point3& point, Rorn::XML::HierarchyElement& parentElement);
	void ExportMaterials(Rorn::XML::HierarchyElement& parentElement);
	void ExportMaterial(Mtl* material, int id, Rorn::XML::HierarchyElement& materialElement);
	void ExportStandardMaterial(StdMat* material, Rorn::XML::HierarchyElement& materialElement);

	Interface* maxInterface_;
	Rorn::XML::DocumentBuilder xmlDocBuilder_;
	std::map<Mtl*, int> materials_;
	std::vector<Mtl*> orderedMaterials_;
	int numNodesExported_;
	int numMaterialsExported_;
};

