#pragma once

#include "../../Interfaces/StreamReader.h"

#include "../../Exceptions/bounds_load_exception.h"

#include "BulletPhysicsSystem.h"
#include "BulletBoxPrimitive.h"

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
	dynamicsWorld_.setGravity(btVector3(0, 0, -10));
}

BulletPhysicsSystem::~BulletPhysicsSystem()
{
	
}

void BulletPhysicsSystem::Step(float timeElapsed)
{
	if(isEnabled_)
		dynamicsWorld_.stepSimulation(timeElapsed);
}

/*virtual*/ unsigned int BulletPhysicsSystem::CreateBoundsInstance(unsigned int boundsId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
{
	BulletBounds* bounds = bounds_[boundsId].get();
	btCollisionShape* collisionShape = bounds->GetCollisionShape();
	float mass = bounds->GetMass();
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		collisionShape->calculateLocalInertia(mass,localInertia);

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

/*virtual*/ void BulletPhysicsSystem::GetBoundsInstanceToWorldTransform(unsigned int boundsInstanceId, Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
{
	BulletBoundsInstance* boundsInstance = boundsInstances_[boundsInstanceId].get();
	boundsInstance->GetInstanceToWorldMatrix(instanceToWorldMatrix);
}

/*virtual*/ unsigned int BulletPhysicsSystem::LoadBounds(unsigned int numPhysicsPrimitives, StreamReader& fileReader)
{
	if(numPhysicsPrimitives == 0)
		return nullPhysicsId_;

	if(numPhysicsPrimitives > 1)
		throw bounds_load_exception("Attempt to load model with more than one physics primitive.  This isn't supported (yet).");

	float mass = fileReader.ReadFloat();
	for(int i = 0 ; i < 16 ; ++i)//skip the primitiveToModelTransform (for now)
		fileReader.ReadFloat();
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
	else
	{
		throw bounds_load_exception("Attempt to load model with unsupported primitive type.");
	}
}