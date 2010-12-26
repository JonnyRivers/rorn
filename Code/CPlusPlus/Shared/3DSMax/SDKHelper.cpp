#include "SDKHelper.h"

bool Rorn::Max::NodeIsToBeIgnored(INode* node)
{
	// Nodes whose names start with an '_' are ignored by the exporter.
	char* nodeName = node->GetName();

	return (nodeName[0] == '_');
}

bool Rorn::Max::IsMeshNode(INode* node)
{
	Object* nodeObject = node->EvalWorldState(0).obj;
	return (nodeObject->CanConvertToType(triObjectClassID) != 0);
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