#include "SceneEnumerator.h"

#include <string>

SceneEnumerator::SceneEnumerator()
	: documentBuilder_("Scene")
{
}


SceneEnumerator::~SceneEnumerator(void)
{
}

/*virtual*/ int SceneEnumerator::callback(INode* node)
{
	XMLHierarchyElement& nodeElement = documentBuilder_.GetRootElement().AddChildHierarchyElement("Node");
	nodeElement.AddChildValueElement("Name", node->GetName());
	Matrix3 m3Node = node->GetNodeTM(0);

	return TREE_CONTINUE;
}

const XMLDocumentBuilder& SceneEnumerator::GetDocumentBuilder() const
{
	return documentBuilder_;
}