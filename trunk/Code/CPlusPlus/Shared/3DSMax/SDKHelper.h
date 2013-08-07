#pragma once

#include <Max.h>
#include <stdmat.h>

// A set of methods that simplify coding against the 3DS Max SDK, preventing code duplication.

namespace Rorn
{
	namespace Max
	{
		bool NodeIsToBeIgnored(INode* node);
		bool IsBoxNode(INode* node);
		bool IsCylinderNode(INode* node);
		bool IsMeshNode(INode* node);
		bool IsOmniLightNode(INode* node);
		bool IsPhysicsNode(INode* node);
		bool IsSphereNode(INode* node);
		Mesh& GetMeshFromNode(INode* node);
		Mtl* GetNodeMaterial(INode* meshNode, MtlID materialID);
		bool IsStandardMaterial(Mtl* material);
		bool HasDiffuseBitmap(Mtl* material);
		BitmapTex* GetDiffuseBitmap(Mtl* material);
	}
}