#pragma once

#include <cassert>

#include "../../Interfaces/StreamReader.h"

#include "../../Exceptions/bounds_load_exception.h"

#include "BulletPhysicsSystem.h"
#include "BulletBoxPrimitive.h"
#include "BulletCompositeBound.h"
#include "BulletCylinderPrimitive.h"
#include "BulletMeshPrimitive.h"
#include "BulletMeshPrimitiveBuilder.h"
#include "BulletSpherePrimitive.h"

using namespace Rorn::Engine;

BulletPhysicsSystem::BulletPhysicsSystem(IDiagnostics* diagnostics) : 
	diagnostics_(diagnostics), 
	isEnabled_(true),
	collisionConfiguration_(), 
	collisionDispatcher_(&collisionConfiguration_), 
	broadphase_(),
	solver_(),
	dynamicsWorld_(&collisionDispatcher_, &broadphase_, &solver_, &collisionConfiguration_)
{
	dynamicsWorld_.setGravity(btVector3(0, -10, 0));
}

BulletPhysicsSystem::~BulletPhysicsSystem()
{
	
}

void BulletPhysicsSystem::Step(float timeElapsed)
{
	if(isEnabled_)
	{
		dynamicsWorld_.stepSimulation(timeElapsed, 20);
	}
}

/*virtual*/ void BulletPhysicsSystem::GetBoundsInstanceToWorldTransform(unsigned int boundsInstanceId, Rorn::Maths::Matrix4x4& instanceToWorldMatrix) const
{
	std::map<unsigned int, std::unique_ptr<BulletBoundsInstance>>::const_iterator boundsInstanceIter = boundsInstances_.find(boundsInstanceId);
	assert(boundsInstanceIter != boundsInstances_.end());
	boundsInstanceIter->second->GetInstanceToWorldMatrix(instanceToWorldMatrix);
}

/*virtual*/ void BulletPhysicsSystem::SetBoundsInstanceLinearVelocity(unsigned int boundsInstanceId, const Rorn::Maths::Vector4& linearVelocity)
{
	std::map<unsigned int, std::unique_ptr<BulletBoundsInstance>>::const_iterator boundsInstanceIter = boundsInstances_.find(boundsInstanceId);
	assert(boundsInstanceIter != boundsInstances_.end());
	boundsInstanceIter->second->SetLinearVelocity(linearVelocity);
}

/*virtual*/ void BulletPhysicsSystem::SetBoundsInstanceToWorldTransform(unsigned int boundsInstanceId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
{
	std::map<unsigned int, std::unique_ptr<BulletBoundsInstance>>::const_iterator boundsInstanceIter = boundsInstances_.find(boundsInstanceId);
	assert(boundsInstanceIter != boundsInstances_.end());
	boundsInstanceIter->second->SetInstanceToWorldMatrix(instanceToWorldMatrix);
}

/*virtual*/ unsigned int BulletPhysicsSystem::CreateBoundsInstance(unsigned int boundsId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
{
	std::map<unsigned int, std::unique_ptr<BulletBounds>>::const_iterator boundsIter = bounds_.find(boundsId);
	assert(boundsIter != bounds_.end());
	btCollisionShape* collisionShape = boundsIter->second->GetCollisionShape();
	float mass = boundsIter->second->GetMass();
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		collisionShape->calculateLocalInertia(mass,localInertia);

	// need to refactor math type conversions
	btTransform instanceToWorldTransform;
	instanceToWorldTransform.setOrigin(btVector3(instanceToWorldMatrix.M41, instanceToWorldMatrix.M42, instanceToWorldMatrix.M43));
	instanceToWorldTransform.setBasis(
		btMatrix3x3(instanceToWorldMatrix.M11, instanceToWorldMatrix.M12, instanceToWorldMatrix.M13,
					instanceToWorldMatrix.M21, instanceToWorldMatrix.M22, instanceToWorldMatrix.M23,
					instanceToWorldMatrix.M31, instanceToWorldMatrix.M32, instanceToWorldMatrix.M33));

	unsigned int newId = static_cast<unsigned int>(boundsInstances_.size());
	BulletBoundsInstance* newPrimitive = new BulletBoundsInstance(dynamicsWorld_, mass, instanceToWorldTransform, collisionShape, localInertia);
	boundsInstances_.insert(std::make_pair<unsigned int, std::unique_ptr<BulletBoundsInstance>>(newId, std::unique_ptr<BulletBoundsInstance>(newPrimitive)));

	return newId;
}

/*virtual*/ unsigned int BulletPhysicsSystem::LoadBounds(unsigned int numPhysicsPrimitives, StreamReader& fileReader)
{
	if(numPhysicsPrimitives == 0)
		return nullPhysicsId_;

	if(numPhysicsPrimitives > 1)
	{
		BulletCompositeBound* newPrimitive = new BulletCompositeBound();

		for(int childIndex = 0 ; childIndex < numPhysicsPrimitives ; ++childIndex)
		{
			float mass = fileReader.ReadFloat();
			float primitiveToModelTransformValues[16];
			for(int i = 0 ; i < 16 ; ++i)//skip the primitiveToModelTransform (for now)
				primitiveToModelTransformValues[i] = fileReader.ReadFloat();
			Maths::Matrix4x4 childToParentTransform(
				primitiveToModelTransformValues[0], primitiveToModelTransformValues[1], primitiveToModelTransformValues[2], primitiveToModelTransformValues[3],
				primitiveToModelTransformValues[4], primitiveToModelTransformValues[5], primitiveToModelTransformValues[6], primitiveToModelTransformValues[7],
				primitiveToModelTransformValues[8], primitiveToModelTransformValues[9], primitiveToModelTransformValues[10], primitiveToModelTransformValues[11],
				primitiveToModelTransformValues[12], primitiveToModelTransformValues[13], primitiveToModelTransformValues[14], primitiveToModelTransformValues[15]);

			unsigned int primitiveType = fileReader.ReadUInt();
			if(primitiveType == 0)
			{
				float width = fileReader.ReadFloat();
				float length = fileReader.ReadFloat();
				float height = fileReader.ReadFloat();

				newPrimitive->AddBox(childToParentTransform, mass, width, length, height);
			}
		}

		unsigned int newId = static_cast<unsigned int>(bounds_.size()) + 1;
		bounds_.insert(std::make_pair<unsigned int, std::unique_ptr<BulletBounds>>(newId, std::unique_ptr<BulletBounds>(newPrimitive)));
		return newId;
	}

	float mass = fileReader.ReadFloat();
	float primitiveToModelTransform[16];
	for(int i = 0 ; i < 16 ; ++i)//skip the primitiveToModelTransform (for now)
		primitiveToModelTransform[i] = fileReader.ReadFloat();
	
	unsigned int primitiveType = fileReader.ReadUInt();
	if(primitiveType == 0)
	{
		float width = fileReader.ReadFloat();
		float length = fileReader.ReadFloat();
		float height = fileReader.ReadFloat();

		unsigned int newId = static_cast<unsigned int>(bounds_.size()) + 1;
		BulletBoxPrimitive* newPrimitive = new BulletBoxPrimitive(mass, width, length, height);
		bounds_.insert(std::make_pair<unsigned int, std::unique_ptr<BulletBounds>>(newId, std::unique_ptr<BulletBounds>(newPrimitive)));

		return newId;
	}
	else if(primitiveType == 1)
	{
		float radius = fileReader.ReadFloat();
		float height = fileReader.ReadFloat();

		unsigned int newId = static_cast<unsigned int>(bounds_.size()) + 1;
		BulletCylinderPrimitive* newPrimitive = new BulletCylinderPrimitive(mass, radius, height);
		bounds_.insert(std::make_pair<unsigned int, std::unique_ptr<BulletBounds>>(newId, std::unique_ptr<BulletBounds>(newPrimitive)));

		return newId;
	}
	else if(primitiveType == 2)
	{
		unsigned int newId = static_cast<unsigned int>(bounds_.size()) + 1;

		BulletMeshPrimitiveBuilder meshPrimitiveBuilder(mass);
		unsigned int numVerts = fileReader.ReadUInt();
		for(unsigned int vertIndex = 0 ; vertIndex < numVerts ; vertIndex += 3)
		{
			float vert0X = fileReader.ReadFloat();
			float vert0Y = fileReader.ReadFloat();
			float vert0Z = fileReader.ReadFloat();
			float vert1X = fileReader.ReadFloat();
			float vert1Y = fileReader.ReadFloat();
			float vert1Z = fileReader.ReadFloat();
			float vert2X = fileReader.ReadFloat();
			float vert2Y = fileReader.ReadFloat();
			float vert2Z = fileReader.ReadFloat();
			meshPrimitiveBuilder.AddTriangle(vert0X, vert0Y, vert0Z, vert1X, vert1Y, vert1Z, vert2X, vert2Y, vert2Z);
		}

		BulletMeshPrimitive* newPrimitive = meshPrimitiveBuilder.ToMeshPrimitive();
		bounds_.insert(std::make_pair<unsigned int, std::unique_ptr<BulletBounds>>(newId, std::unique_ptr<BulletBounds>(newPrimitive)));

		return newId;
	}
	else if(primitiveType == 3)
	{
		float radius = fileReader.ReadFloat();

		unsigned int newId = static_cast<unsigned int>(bounds_.size()) + 1;
		BulletSpherePrimitive* newPrimitive = new BulletSpherePrimitive(mass, radius);
		bounds_.insert(std::make_pair<unsigned int, std::unique_ptr<BulletBounds>>(newId, std::unique_ptr<BulletBounds>(newPrimitive)));

		return newId;
	}
	else
	{
		throw bounds_load_exception("Attempt to load model with unsupported primitive type.");
	}
}