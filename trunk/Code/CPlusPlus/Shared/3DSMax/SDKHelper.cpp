#include "SDKHelper.h"

#include <string>

bool Rorn::Max::NodeIsToBeIgnored(INode* node)
{
	// Nodes whose names start with an '_' are ignored by the exporter.
	char* nodeName = node->GetName();

	return (nodeName[0] == '_');
}

bool Rorn::Max::IsBoxNode(INode* node)
{
	Object* nodeObject = node->EvalWorldState(0).obj;

	return (nodeObject->SuperClassID() == GEOMOBJECT_CLASS_ID &&
		    nodeObject->ClassID() == Class_ID(BOXOBJ_CLASS_ID, 0));
}

bool Rorn::Max::IsCylinderNode(INode* node)
{
	Object* nodeObject = node->EvalWorldState(0).obj;

	return (nodeObject->SuperClassID() == GEOMOBJECT_CLASS_ID &&
		    nodeObject->ClassID() == Class_ID(CYLINDER_CLASS_ID, 0));
}

bool Rorn::Max::IsMeshNode(INode* node)
{
	Object* nodeObject = node->EvalWorldState(0).obj;

	return (nodeObject->SuperClassID() == GEOMOBJECT_CLASS_ID &&
		    nodeObject->ClassID() == Class_ID(EDITTRIOBJ_CLASS_ID, 0));
}

bool Rorn::Max::IsOmniLightNode(INode* node)
{
	Object* nodeObject = node->GetObjectRef();

	return (nodeObject->SuperClassID() == LIGHT_CLASS_ID &&
		    nodeObject->ClassID() == Class_ID(OMNI_LIGHT_CLASS_ID, 0));
}

bool Rorn::Max::IsPhysicsNode(INode* node)
{
	std::string nodeName(node->GetName());

	return (nodeName.compare(0, 7, "physics") == 0);
}

bool Rorn::Max::IsSphereNode(INode* node)
{
	Object* nodeObject = node->EvalWorldState(0).obj;

	return (nodeObject->SuperClassID() == GEOMOBJECT_CLASS_ID &&
		    nodeObject->ClassID() == Class_ID(SPHERE_CLASS_ID, 0));
}

Mesh& Rorn::Max::GetMeshFromNode(INode* node)
{
	Object* nodeObject = node->EvalWorldState(0).obj;
	TriObject* triObject = (TriObject*)nodeObject->ConvertToType(0, triObjectClassID);
	return triObject->GetMesh();
}

Mtl* Rorn::Max::GetNodeMaterial(INode* meshNode, MtlID materialID)
{
	Mtl* nodeMaterial = meshNode->GetMtl();

	if(nodeMaterial == NULL)
		return NULL;

	if( !nodeMaterial->IsMultiMtl() )
	{
		return nodeMaterial;
	}
	else
	{
		// Max does some crazy material ID wrapping crap.
		// If a multi material has 5 sub materials then an MtlID of 10 is valid.  
		// It translates to MtlID % NumSubMtls.  So, 10 % 5 == 0.
		int subMaterialIndex = materialID % nodeMaterial->NumSubMtls();
		return nodeMaterial->GetSubMtl(subMaterialIndex);
	}
}

bool Rorn::Max::IsStandardMaterial(Mtl* material)
{
	return (material->IsSubClassOf( Class_ID(DMTL_CLASS_ID,0) ) == TRUE);
}

bool Rorn::Max::HasDiffuseBitmap(Mtl* material)
{
	return (GetDiffuseBitmap(material) != NULL);
}

BitmapTex* Rorn::Max::GetDiffuseBitmap(Mtl* material)
{
	Texmap* diffuseTextureMap = material->GetSubTexmap(ID_DI);
	if( diffuseTextureMap != NULL )
	{
		if (diffuseTextureMap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			BitmapTex* diffuseBitmap = static_cast<BitmapTex*>(diffuseTextureMap);
			return diffuseBitmap;
		}
	}

	return NULL;
}