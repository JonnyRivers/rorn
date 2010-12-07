#include "RornSceneExporter.h"

#include <sstream>

RornSceneExporter::RornSceneExporter() 
{
}

RornSceneExporter::~RornSceneExporter()
{
}

int RornSceneExporter::ExtCount() 
{
	return 1;
}

const TCHAR* RornSceneExporter::Ext(int n) 
{
	switch(n) 
	{
		case 0:
			return _T("Rorn.Scene.Xml");
	}

	return _T("");
}

const TCHAR* RornSceneExporter::LongDesc() 
{			
	return _T("Rorn Engine Scene File");
}
	
const TCHAR* RornSceneExporter::ShortDesc() 
{			
	return _T("Rorn Scene");
}

const TCHAR* RornSceneExporter::AuthorName() 
{			
	return _T("Jonny Rivers");
}

const TCHAR* RornSceneExporter::CopyrightMessage() 
{
	return _T("Copyright 2010 Riversoft");
}

const TCHAR* RornSceneExporter::OtherMessage1() 
{
	return _T("");
}

const TCHAR* RornSceneExporter::OtherMessage2() 
{
	return _T("");
}

unsigned int RornSceneExporter::Version() 
{
	return 1;
}

void RornSceneExporter::ShowAbout(HWND hWnd) 
{
}

int RornSceneExporter::DoExport(const TCHAR* filename, 
								ExpInterface* expInterface, 
								Interface* maxInterface, 
								BOOL suppressPrompts, 
								DWORD options) 
{
	XMLDocumentBuilder xmlDocBuilder("Scene");
	INode* rootNode = maxInterface->GetRootNode();
	ExportNodeRecursive(rootNode, xmlDocBuilder.GetRootElement());
	xmlDocBuilder.Save(filename);

	return IMPEXP_SUCCESS;
}

BOOL RornSceneExporter::SupportsOptions(int ext, DWORD options) 
{
	assert(ext == 0);

	return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}

/*static*/ void RornSceneExporter::ExportNodeRecursive(INode* parentNode, XMLHierarchyElement& parentElement)
{
	if(parentNode != GetCOREInterface()->GetRootNode())
	{
		XMLHierarchyElement& newNodeElement = parentElement.AddChildHierarchyElement("Node");

		ExportNode(parentNode, newNodeElement);

		for(int childIndex = 0 ; childIndex < parentNode->NumberOfChildren() ; ++childIndex)
		{
			ExportNodeRecursive(parentNode->GetChildNode(childIndex), newNodeElement);
		}
	}
	else
	{
		// The root node is a special case, here we just pass the root element through to its children
		for(int childIndex = 0 ; childIndex < parentNode->NumberOfChildren() ; ++childIndex)
		{
			ExportNodeRecursive(parentNode->GetChildNode(childIndex), parentElement);
		}
	}
}

/*static*/ void RornSceneExporter::ExportNode(INode* node, XMLHierarchyElement& nodeElement)
{
	nodeElement.AddChildValueElement("Name", node->GetName());
	ExportMaterial(node->GetMtl(), nodeElement);

	Object* nodeObject = node->EvalWorldState(0).obj;
	if (nodeObject->CanConvertToType(triObjectClassID))// is it a mesh?
	{
		TriObject* triObject = (TriObject*)nodeObject->ConvertToType(0, triObjectClassID);
		Mesh& mesh = triObject->GetMesh();
		XMLHierarchyElement& meshElement = nodeElement.AddChildHierarchyElement("Mesh");
		ExportMesh(mesh, meshElement);
	}
}

/*static*/ void RornSceneExporter::ExportMaterial(Mtl* material, XMLHierarchyElement& parentElement)
{
	if(material == NULL)
		return;// nothing to export!

	if(material->IsMultiMtl())
	{
		XMLHierarchyElement& multiMtl = parentElement.AddChildHierarchyElement("MultiMtl");
		for(int childMtlIndex = 0 ; childMtlIndex != material->NumSubMtls() ; ++childMtlIndex)
		{
			Mtl* childMaterial = material->GetSubMtl(childMtlIndex);
			ExportMaterial(childMaterial, parentElement);
		}
	}
	else
	{
		parentElement.AddChildValueElement("Name", material->GetName());
		// TODO - We want to export colours and texture information
	}
}

/*static*/ void RornSceneExporter::ExportMesh(Mesh& mesh, XMLHierarchyElement& meshElement)
{
	for(int vertIndex = 0 ; vertIndex < mesh.numVerts ; ++vertIndex)
	{
		std::stringstream name;
		name  << "Vertex" << vertIndex;

		ExportPoint(name.str().c_str(), mesh.verts[vertIndex], meshElement);
	}

	for(int faceIndex = 0 ; faceIndex < mesh.numFaces ; ++faceIndex)
	{
		std::stringstream name;
		name  << "Face" << faceIndex;

		ExportFace(name.str().c_str(), mesh.faces[faceIndex], meshElement);
	}

	// TODO - We also want to export UVs, colours and normals
}
/*static*/ void RornSceneExporter::ExportPoint(const char* name, const Point3& point, XMLHierarchyElement& meshElement)
{
	XMLHierarchyElement& pointElement = meshElement.AddChildHierarchyElement(name);

	std::stringstream xValue;
	xValue << point.x;
	pointElement.AddChildValueElement("X", xValue.str().c_str());

	std::stringstream yValue;
	yValue << point.y;
	pointElement.AddChildValueElement("Y", yValue.str().c_str());

	std::stringstream zValue;
	zValue << point.z;
	pointElement.AddChildValueElement("Z", zValue.str().c_str());
}

/*static*/ void RornSceneExporter::ExportFace(const char* name, Face& face, XMLHierarchyElement& meshElement)
{
	XMLHierarchyElement& faceElement = meshElement.AddChildHierarchyElement(name);

	std::stringstream oneValue;
	oneValue << face.v[0];
	faceElement.AddChildValueElement("One", oneValue.str().c_str());

	std::stringstream twovalue;
	twovalue << face.v[1];
	faceElement.AddChildValueElement("Two", twovalue.str().c_str());

	std::stringstream threeValue;
	threeValue << face.v[2];
	faceElement.AddChildValueElement("Three", threeValue.str().c_str());

	std::stringstream matIdValue;
	matIdValue << face.getMatID();
	faceElement.AddChildValueElement("MatId", matIdValue.str().c_str());
}
