#pragma once

#include <string>
#include <vector>

#include <Max.h>
#include <stdmat.h>

class SceneValidator
{
public:
	SceneValidator(Interface* maxInterface);
	~SceneValidator(void);

	bool ValidateScene();
private:
	void ValidateNodeRecursive(INode* parentNode);
	void ValidateNode(INode* node);
	void ValidateMeshNode(INode* meshNode);
	bool ValidatePhysicsNode(INode* physicsNode);
	void ValidateMaterial(Mtl* material, INode* meshNode);
	void ValidateStandardMaterial(StdMat* standardMaterial);

	Interface* maxInterface_;
	std::vector<std::string> errors_;
	std::vector<std::string> warnings_;
	std::vector<INode*> meshNodes_;
};

