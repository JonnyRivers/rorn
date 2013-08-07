#include "SceneValidator.h"

#include <map>
#include <set>
#include <sstream>

#include "../../../Shared/3DSMax/SDKHelper.h"

SceneValidator::SceneValidator(Interface* maxInterface)
	: maxInterface_(maxInterface)
{
}

SceneValidator::~SceneValidator(void)
{
}

bool SceneValidator::ValidateScene()
{
	// Reset data from previous runs
	errors_.clear();
	warnings_.clear();
	meshNodes_.clear();

	INode* rootNode = maxInterface_->GetRootNode();
	ValidateNodeRecursive(rootNode);

	// Output any errors
	if(!errors_.empty())
	{
		std::stringstream errorDisplayStream;
		errorDisplayStream << "Unable to export scene because of the following errors:" << std::endl;
		std::vector<std::string>::const_iterator errorIt;
		for(errorIt = errors_.begin() ; errorIt != errors_.end() ; ++errorIt)
			errorDisplayStream << '\t' << *errorIt << std::endl;

		::MessageBoxA(NULL, errorDisplayStream.str().c_str(), "Export failed", MB_OK);
	}

	return errors_.empty();
}

void SceneValidator::ValidateNodeRecursive(INode* parentNode)
{
	if(parentNode != GetCOREInterface()->GetRootNode())
	{
		ValidateNode(parentNode);

		for(int childIndex = 0 ; childIndex < parentNode->NumberOfChildren() ; ++childIndex)
		{
			ValidateNodeRecursive(parentNode->GetChildNode(childIndex));
		}
	}
	else
	{
		// The root node is a special case, here we just pass the root element through to its children
		for(int childIndex = 0 ; childIndex < parentNode->NumberOfChildren() ; ++childIndex)
		{
			ValidateNodeRecursive(parentNode->GetChildNode(childIndex));
		}
	}
}

void SceneValidator::ValidateNode(INode* node)
{
	if(Rorn::Max::NodeIsToBeIgnored(node))
		return;

	if(Rorn::Max::IsPhysicsNode(node))
	{
		ValidatePhysicsNode(node);
	}
	else if(Rorn::Max::IsMeshNode(node))
	{
		ValidateMeshNode(node);
	}
	else
	{
		std::stringstream errorStream;
		errorStream << "Unable to export node '" << node->GetName() << 
			"' as only mesh nodes can be exported.  " <<
			"If you need the exporter to ignore particular nodes then prefix the name with an underscore '_' character.";
		errors_.push_back(errorStream.str());
	}
}

void SceneValidator::ValidateMeshNode(INode* meshNode)
{
	if( meshNode->GetMtl() == NULL )
	{
		std::stringstream errorStream;
		errorStream << "Unable to export node '" << meshNode->GetName() << 
			"' as it apepars to be a mesh node but doesn't have a material assigned to it.  " <<
			"If you need the exporter to ignore particular nodes then prefix the name with an underscore '_' character.";
		errors_.push_back(errorStream.str());
		return;
	}

	// Here we check that any materials referenced are valid
	Mesh& mesh = Rorn::Max::GetMeshFromNode(meshNode);

	if( mesh.numFaces == 0 )
	{
		std::stringstream errorStream;
		errorStream << "Unable to export node '" << meshNode->GetName() << 
			"' as it appears to be a mesh node but doesn't contain any faces.  " <<
			"If you need the exporter to ignore particular nodes then prefix the name with an underscore '_' character.";
		errors_.push_back(errorStream.str());
		return;
	}

	// pull out all referenced materials (invalid sub-materials that aren't referenced are fine)
	std::set<Mtl*> referencedMaterials;
	for(int faceIndex = 0 ; faceIndex < mesh.numFaces ; ++faceIndex)
		referencedMaterials.insert(Rorn::Max::GetNodeMaterial(meshNode, mesh.faces[faceIndex].getMatID()));

	// Validate the materials
	std::set<Mtl*>::const_iterator iter;
	for(iter = referencedMaterials.begin() ; iter != referencedMaterials.end() ; ++iter)
		ValidateMaterial(*iter, meshNode);
}

void SceneValidator::ValidateMaterial(Mtl* material, INode* meshNode)
{
	if(Rorn::Max::IsStandardMaterial(material))
	{
		ValidateStandardMaterial(static_cast<StdMat*>(material));
	}
	else
	{
		std::stringstream errorStream;
		errorStream << "Unable to export node '" << meshNode->GetName() << "' as only standard materials can be exported.  "
					<< "Material '" << material->GetName() << "' is not a standard material.";
		errors_.push_back(errorStream.str());
	}
}

bool SceneValidator::ValidatePhysicsNode(INode* physicsNode)
{
	if( !Rorn::Max::IsBoxNode(physicsNode) &&
		!Rorn::Max::IsCylinderNode(physicsNode) &&
		!Rorn::Max::IsMeshNode(physicsNode) &&
		!Rorn::Max::IsSphereNode(physicsNode))
	{
		std::stringstream errorStream;
		errorStream << "Unable to export node '" << physicsNode->GetName() << "' as only boxes, cylinders, meshes and spheres can be exported as physics objects.";
		errors_.push_back(errorStream.str());

		return false;
	}

	return true;
}

void SceneValidator::ValidateStandardMaterial(StdMat* standardMaterial)
{
	// No further validtaion required atm
}