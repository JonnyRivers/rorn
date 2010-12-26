#pragma once

#include <Max.h>

// A set of methods that simplify coding against the 3DS Max SDK, preventing code duplication.

namespace Rorn
{
	namespace Max
	{
		bool NodeIsToBeIgnored(INode* node);
		bool IsMeshNode(INode* node);
		Mesh& GetMeshFromNode(INode* node);
		Mtl* GetNodeMaterial(INode* meshNode, MtlID materialID);
		bool IsStandardMaterial(Mtl* material);
	}
}