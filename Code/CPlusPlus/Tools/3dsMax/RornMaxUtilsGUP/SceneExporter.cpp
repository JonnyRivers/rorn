#include "SceneExporter.h"

#include <sstream>

#include "../../../Shared/3DSMax/SDKHelper.h"
#include "../../../Shared/3DSMax/BoxPrimitive.h"
#include "../../../Shared/3DSMax/CylinderPrimitive.h"
#include "../../../Shared/3DSMax/SpherePrimitive.h"
#include "../../../Shared/Text/Path.h"

namespace
{
	const int maxNormalsToSearchThrough = 4;
}

SceneExporter::SceneExporter(Interface* maxInterface)
	: maxInterface_(maxInterface),
	  xmlDocBuilder_("Scene"),
	  numNodesExported_(0),
	  numMaterialsExported_(0)
{
}

SceneExporter::~SceneExporter(void)
{
}

void SceneExporter::ExportScene(const Rorn::Text::ci_string& exportPathName)
{
	// Reset the state (to allow multiple uses)
	xmlDocBuilder_ = Rorn::XML::DocumentBuilder("Scene");
	materials_.clear();
	orderedMaterials_.clear();
	numNodesExported_ = 0;
	numMaterialsExported_ = 0;
	textureExporter_.Reset();

	xmlDocBuilder_.GetRootElement().AddChildValueElement("Source", "3DS Max 2010");
	INode* rootNode = maxInterface_->GetRootNode();
	ExportNodeRecursive(rootNode, xmlDocBuilder_.GetRootElement());// materials_ is built up here
	ExportMaterials(xmlDocBuilder_.GetRootElement());
	xmlDocBuilder_.Save(exportPathName.c_str());

	Rorn::Text::ci_string exportDirectory = Rorn::Text::Path::GetDirectoryFromPathName(exportPathName);
	textureExporter_.CopyTexturesToDirectory(exportDirectory.c_str());
}

void SceneExporter::ExportNodeRecursive(INode* parentNode, Rorn::XML::HierarchyElement& parentElement)
{
	if(parentNode != GetCOREInterface()->GetRootNode())
	{
		Rorn::XML::HierarchyElement& newNodeElement = parentElement.AddChildHierarchyElement("Node");

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


void SceneExporter::ExportNode(INode* node, Rorn::XML::HierarchyElement& nodeElement)
{
	nodeElement.AddChildValueElement("Name", node->GetName());
	std::stringstream idStream;
	idStream << numNodesExported_++;
	nodeElement.AddChildValueElement("Id", idStream.str().c_str());
	ExportMatrix3("NodeToSceneMatrix", node->GetNodeTM(0), nodeElement);

	if( Rorn::Max::IsPhysicsNode(node) )
	{
		ExportPhysics(node, nodeElement);
	}
	else if(Rorn::Max::IsMeshNode(node))
	{
		Mesh& mesh = Rorn::Max::GetMeshFromNode(node);
		Rorn::XML::HierarchyElement& meshElement = nodeElement.AddChildHierarchyElement("Mesh");
		ExportMesh(node, mesh, meshElement);
	}
}

void SceneExporter::ExportMatrix3(const char* name, const Matrix3& matrix, Rorn::XML::HierarchyElement& parentElement)
{
	std::stringstream valueStream;
	valueStream << matrix.GetRow(0).x << "," << matrix.GetRow(0).y << "," << matrix.GetRow(0).z << ","
				<< matrix.GetRow(1).x << "," << matrix.GetRow(1).y << "," << matrix.GetRow(1).z << ","
				<< matrix.GetRow(2).x << "," << matrix.GetRow(2).y << "," << matrix.GetRow(2).z << ","
				<< matrix.GetRow(3).x << "," << matrix.GetRow(3).y << "," << matrix.GetRow(3).z;
	parentElement.AddChildValueElement(name, valueStream.str().c_str());
}

void SceneExporter::ExportMesh(INode* node, Mesh& mesh, Rorn::XML::HierarchyElement& meshElement)
{
	mesh.buildNormals();

	// Export vertices
	int vertsExported = 0;
	for(int faceIndex = 0 ; faceIndex < mesh.numFaces ; ++faceIndex)
	{
		for(int vertIndex = 0 ; vertIndex < 3 ; ++vertIndex)
		{
			Rorn::XML::HierarchyElement& vertexElement = meshElement.AddChildHierarchyElement("Vertex");
			std::stringstream indexStream;
			indexStream << vertsExported++;
			vertexElement.AddChildValueElement("Index", indexStream.str().c_str());
			ExportPoint3("Position", mesh.verts[mesh.faces[faceIndex].v[vertIndex]], vertexElement);

			// Export normal
			RVertex& rVertex = mesh.getRVert(mesh.faces[faceIndex].v[vertIndex]);
			if(rVertex.ern == NULL)
			{
				// If ern is NULL then there is only one computed normal.  What could be easier?
				ExportPoint3("Normal", rVertex.rn.getNormal(), vertexElement);
			}
			else
			{
				// Here we have multiple normals for this vertex so we have to take the normal with the matching
				// smoothing group
				bool foundNormal = false;
				DWORD faceSmGroup = mesh.faces[faceIndex].getSmGroup();
				int normalIndex = 0;
				while(!foundNormal && normalIndex < maxNormalsToSearchThrough)// There must be a better way.  Max doesn't seem to store a limit.
				{
					
					if(rVertex.ern[normalIndex].getSmGroup() == faceSmGroup)
					{
						ExportPoint3("Normal", rVertex.ern[normalIndex].getNormal(), vertexElement);
						foundNormal = true;
					}

					++normalIndex;
				}

				if(!foundNormal)
				{
					ExportPoint3("Normal", rVertex.rn.getNormal(), vertexElement);
				}
			}

			// Export texture coordinates (diffuse only for now)
			if(mesh.maps[1].flags & MESHMAP_USED)
			{
				DWORD tvIndex = mesh.maps[1].tf[faceIndex].t[vertIndex];
				ExportPoint3("DiffuseUVW", mesh.maps[1].tv[tvIndex], vertexElement);
			}
		}
	}

	// export triangles
	vertsExported = 0; 
	for(int faceIndex = 0 ; faceIndex < mesh.numFaces ; ++faceIndex)
	{
		// Get the material for this face and add it to our collection (if necessary)
		Mtl* material = Rorn::Max::GetNodeMaterial(node, mesh.faces[faceIndex].getMatID());
		std::map<Mtl*, int>::const_iterator findIter = materials_.find(material);
		int materialId;
		if( findIter != materials_.end())
		{
			materialId = findIter->second;// get the id
		}
		else
		{
			materialId = materials_[material] = numMaterialsExported_++;// add it to the collection
			orderedMaterials_.push_back(material);// to allow ordered export
		}

		std::stringstream valueStream;
		valueStream << vertsExported << "," << vertsExported + 1 << "," << vertsExported + 2 << "," << materialId;
		vertsExported += 3;
		meshElement.AddChildValueElement("Triangle", valueStream.str().c_str());
	}
}

void SceneExporter::ExportPoint3(const char* name, const Point3& point, Rorn::XML::HierarchyElement& parentElement)
{
	std::stringstream valueStream;
	valueStream << point.x << "," << point.y << "," << point.z;
	parentElement.AddChildValueElement(name, valueStream.str().c_str());
}

void SceneExporter::ExportPhysics(INode* node, Rorn::XML::HierarchyElement& nodeElement)
{
	MSTR massAsString = "0";
	node->GetUserPropString("mass", massAsString);

	if( Rorn::Max::IsBoxNode(node) )
	{
		Rorn::Max::BoxPrimitive boxPrimitive(node);

		Rorn::XML::HierarchyElement& boxElement = nodeElement.AddChildHierarchyElement("Box");

		boxElement.AddChildValueElement("Mass", massAsString);

		std::stringstream widthValueStream;
		widthValueStream << boxPrimitive.GetWidth();
		boxElement.AddChildValueElement("Width", widthValueStream.str().c_str());

		std::stringstream lengthValueStream;
		lengthValueStream << boxPrimitive.GetLength();
		boxElement.AddChildValueElement("Length", lengthValueStream.str().c_str());

		std::stringstream heightValueStream;
		heightValueStream << boxPrimitive.GetHeight();
		boxElement.AddChildValueElement("Height", heightValueStream.str().c_str());
	}
	else if( Rorn::Max::IsCylinderNode(node) )
	{
		Rorn::Max::CylinderPrimitive cylinderPrimitive(node);

		Rorn::XML::HierarchyElement& cylinderElement = nodeElement.AddChildHierarchyElement("Cylinder");

		cylinderElement.AddChildValueElement("Mass", massAsString);

		std::stringstream radiusValueStream;
		radiusValueStream << cylinderPrimitive.GetRadius();
		cylinderElement.AddChildValueElement("Radius", radiusValueStream.str().c_str());

		std::stringstream heightValueStream;
		heightValueStream << cylinderPrimitive.GetHeight();
		cylinderElement.AddChildValueElement("Height", heightValueStream.str().c_str());
	}
	else if( Rorn::Max::IsMeshNode(node) )
	{
		Mesh& mesh = Rorn::Max::GetMeshFromNode(node);

		//meshElement.AddChildValueElement("Mass", massAsString);
	}
	else if( Rorn::Max::IsSphereNode(node) )
	{
		Rorn::Max::SpherePrimitive spherePrimitive(node);

		Rorn::XML::HierarchyElement& sphereElement = nodeElement.AddChildHierarchyElement("Sphere");

		sphereElement.AddChildValueElement("Mass", massAsString);

		std::stringstream radiusValueStream;
		radiusValueStream << spherePrimitive.GetRadius();
		sphereElement.AddChildValueElement("Radius", radiusValueStream.str().c_str());
	}
}

void SceneExporter::ExportMaterials(Rorn::XML::HierarchyElement& parentElement)
{
	// Use our orderedMaterials_ container to export in id order
	std::vector<Mtl*>::const_iterator materialIter;
	for(materialIter = orderedMaterials_.begin() ; materialIter != orderedMaterials_.end() ; ++materialIter)
	{
		// find the entry in the map to pull ou the index (could iterate, but this feels tighter)
		std::map<Mtl*, int>::const_iterator findIter = materials_.find(*materialIter);
		Rorn::XML::HierarchyElement& materialElement = parentElement.AddChildHierarchyElement("Material");
		ExportMaterial(findIter->first, findIter->second, materialElement);
	}
}

void SceneExporter::ExportMaterial(Mtl* material, int id, Rorn::XML::HierarchyElement& materialElement)
{
	std::stringstream idStream;
	idStream << id;
	materialElement.AddChildValueElement("Id", idStream.str().c_str());
	ExportPoint3("AmbientColor", material->GetAmbient(), materialElement);
	ExportPoint3("DiffuseColor", material->GetDiffuse(), materialElement);
	ExportPoint3("SpecularColor", material->GetSpecular(), materialElement);
	std::stringstream phongExponentStream;
	phongExponentStream << (material->GetShininess() * 100.0f);
	materialElement.AddChildValueElement("PhongExponent", phongExponentStream.str().c_str());
	
	if(Rorn::Max::IsStandardMaterial(material))
	{
		ExportStandardMaterial(static_cast<StdMat*>(material), materialElement);
	}
}

void SceneExporter::ExportStandardMaterial(StdMat* material, Rorn::XML::HierarchyElement& materialElement)
{
	if( Rorn::Max::HasDiffuseBitmap(material) )
	{
		Rorn::XML::HierarchyElement& textureMapsElement = materialElement.AddChildHierarchyElement("TextureMaps");

		BitmapTex* diffuseBitmap = Rorn::Max::GetDiffuseBitmap(material);
		ExportDiffuseBitmap(diffuseBitmap, textureMapsElement);
	}
}

void SceneExporter::ExportDiffuseBitmap(BitmapTex* diffuseBitmap, Rorn::XML::HierarchyElement& textureMapsElement)
{
	Rorn::XML::HierarchyElement& diffuseMapElement = textureMapsElement.AddChildHierarchyElement("DiffuseMap");
	Rorn::Text::ci_string targetTexturePathName = textureExporter_.RegisterTexture(diffuseBitmap->GetMapName());
	diffuseMapElement.AddChildValueElement("PathName", targetTexturePathName.c_str());
}