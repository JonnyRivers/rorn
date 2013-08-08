#pragma once

#include "BulletBoundsInstance.h"

using namespace Rorn::Engine;

BulletBoundsInstance::BulletBoundsInstance(btDiscreteDynamicsWorld& dynamicsWorld, float mass, const btTransform& instanceToWorldTransform, btCollisionShape* collisionShape, const btVector3& localInertia) :
	motionState_(instanceToWorldTransform),
	rigidBody_( btRigidBody::btRigidBodyConstructionInfo(mass, &motionState_, collisionShape, localInertia) )
{
	//add the body to the dynamics world
	dynamicsWorld.addRigidBody(&rigidBody_);
}

BulletBoundsInstance::~BulletBoundsInstance()
{
	
}

void BulletBoundsInstance::GetInstanceToWorldMatrix(Rorn::Maths::Matrix4x4& instanceToWorldMatrix) const
{
	// need to refactor math type conversions
	const btTransform& localToWorldTransform = rigidBody_.getWorldTransform();
	const btMatrix3x3& basis = localToWorldTransform.getBasis();
	const btVector3& basisRow1 = basis[0];
	const btVector3& basisRow2 = basis[1];
	const btVector3& basisRow3 = basis[2];
	const btVector3& origin = localToWorldTransform.getOrigin();

	instanceToWorldMatrix.M11 = basisRow1.x();
	instanceToWorldMatrix.M12 = basisRow2.x();
	instanceToWorldMatrix.M13 = basisRow3.x();
	instanceToWorldMatrix.M14 = 0;
	instanceToWorldMatrix.M21 = basisRow1.y();
	instanceToWorldMatrix.M22 = basisRow2.y();
	instanceToWorldMatrix.M23 = basisRow3.y();
	instanceToWorldMatrix.M24 = 0;
	instanceToWorldMatrix.M31 = basisRow1.z();
	instanceToWorldMatrix.M32 = basisRow2.z();
	instanceToWorldMatrix.M33 = basisRow3.z();
	instanceToWorldMatrix.M34 = 0;
	instanceToWorldMatrix.M41 = origin.x();
	instanceToWorldMatrix.M42 = origin.y();
	instanceToWorldMatrix.M43 = origin.z();
	instanceToWorldMatrix.M44 = 1;
}

void BulletBoundsInstance::SetInstanceToWorldMatrix(const Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
{
	// need to refactor math type conversions
	btTransform instanceToWorldTransform;
	instanceToWorldTransform.setOrigin(btVector3(instanceToWorldMatrix.M41, instanceToWorldMatrix.M42, instanceToWorldMatrix.M43));
	instanceToWorldTransform.setBasis(
		btMatrix3x3(instanceToWorldMatrix.M11, instanceToWorldMatrix.M12, instanceToWorldMatrix.M13,
					instanceToWorldMatrix.M21, instanceToWorldMatrix.M22, instanceToWorldMatrix.M23,
					instanceToWorldMatrix.M31, instanceToWorldMatrix.M32, instanceToWorldMatrix.M33));

	rigidBody_.setWorldTransform(instanceToWorldTransform);
}

void BulletBoundsInstance::SetLinearVelocity(const Rorn::Maths::Vector4& linearVelocity)
{
	rigidBody_.setLinearVelocity(btVector3(linearVelocity.X, linearVelocity.Y, linearVelocity.Z));
}
