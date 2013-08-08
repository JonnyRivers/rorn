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
	const btTransform& localToWorldTransform = rigidBody_.getWorldTransform();
	const btMatrix3x3& basis = localToWorldTransform.getBasis();
	const btVector3& basisRow1 = basis[0];
	const btVector3& basisRow2 = basis[1];
	const btVector3& basisRow3 = basis[2];
	const btVector3& origin = localToWorldTransform.getOrigin();

	instanceToWorldMatrix.M11 = basisRow1.x();
	instanceToWorldMatrix.M12 = basisRow1.y();
	instanceToWorldMatrix.M13 = basisRow1.z();
	instanceToWorldMatrix.M14 = 0;
	instanceToWorldMatrix.M21 = basisRow2.x();
	instanceToWorldMatrix.M22 = basisRow2.y();
	instanceToWorldMatrix.M23 = basisRow2.z();
	instanceToWorldMatrix.M24 = 0;
	instanceToWorldMatrix.M31 = basisRow3.x();
	instanceToWorldMatrix.M32 = basisRow3.y();
	instanceToWorldMatrix.M33 = basisRow3.z();
	instanceToWorldMatrix.M34 = 0;
	instanceToWorldMatrix.M41 = origin.x();
	instanceToWorldMatrix.M42 = origin.y();
	instanceToWorldMatrix.M43 = origin.z();
	instanceToWorldMatrix.M44 = 1;
}