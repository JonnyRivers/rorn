#include "SceneExporter.h"


SceneExporter::SceneExporter(Interface* maxInterface)
{
}


SceneExporter::~SceneExporter(void)
{
}

void SceneExporter::ExportScene(const std::string& filename)
{
}

// DEPRECATED CODE - KILL WHEN FINISHED

/*
void RornSceneExporter::ExportScene(const TCHAR* filename, Interface* maxInterface)
{
	XMLDocumentBuilder xmlDocBuilder("Scene");
	INode* rootNode = maxInterface->GetRootNode();
	ExportNodeRecursive(rootNode, xmlDocBuilder.GetRootElement());
	xmlDocBuilder.GetRootElement().AddChildValueElement("Source", "3DS Max 2010");
	xmlDocBuilder.Save(filename);
}

void RornSceneExporter::ExportNodeRecursive(INode* parentNode, XMLHierarchyElement& parentElement)
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

void RornSceneExporter::ExportNode(INode* node, XMLHierarchyElement& nodeElement)
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

void RornSceneExporter::ExportMaterial(Mtl* material, XMLHierarchyElement& parentElement)
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

void RornSceneExporter::ExportMesh(Mesh& mesh, XMLHierarchyElement& meshElement)
{
	mesh.buildNormals();

	for(int vertIndex = 0 ; vertIndex < mesh.numVerts ; ++vertIndex)
	{
		std::stringstream name;
		name  << "Vertex" << vertIndex;
		XMLHierarchyElement& vertexElement = meshElement.AddChildHierarchyElement(name.str().c_str());

		ExportPoint("Position", mesh.verts[vertIndex], vertexElement);
		ExportPoint("Normal", mesh.gfxNormals[vertIndex], vertexElement);
	}

	for(int faceIndex = 0 ; faceIndex < mesh.numFaces ; ++faceIndex)
	{
		std::stringstream name;
		name  << "Face" << faceIndex;

		ExportFace(name.str().c_str(), mesh.faces[faceIndex], meshElement);
	}

	
}

void RornSceneExporter::ExportPoint(const char* name, const Point3& point, XMLHierarchyElement& meshElement)
{
	XMLHierarchyElement& pointElement = meshElement.AddChildHierarchyElement(name);

	std::stringstream value;
	value << point.x << "," << point.y << "," << point.z;
	pointElement.AddChildValueElement(name, value.str().c_str());
}

void RornSceneExporter::ExportFace(const char* name, Face& face, XMLHierarchyElement& meshElement)
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
}*/