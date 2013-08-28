#pragma once

#include "BulletCompositeBound.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

BulletCompositeBound::BulletCompositeBound() :
	BulletBounds(0), 
	compundShape_(),
	childShapes_()
{
	
}

BulletCompositeBound::~BulletCompositeBound()
{
	
}

void BulletCompositeBound::AddBox(const Matrix4x4& childToParentTransform, float mass, float width, float length, float height)
{
	mass_ += mass;
	btCollisionShape* newChildShape = new btBoxShape(btVector3(width * 0.5f, height * 0.5, length * 0.5));
	childShapes_.push_back(std::unique_ptr<btCollisionShape>(newChildShape));

	// need to refactor math type conversions
	btTransform childToParentTransformBullet(
		btMatrix3x3(childToParentTransform.M11, childToParentTransform.M12, childToParentTransform.M13,
					childToParentTransform.M21, childToParentTransform.M22, childToParentTransform.M23,
					childToParentTransform.M31, childToParentTransform.M32, childToParentTransform.M33),
		btVector3(childToParentTransform.M41, childToParentTransform.M42, childToParentTransform.M43));
	compundShape_.addChildShape(childToParentTransformBullet, newChildShape);
}

/*virtual*/ btCollisionShape* BulletCompositeBound::GetCollisionShape()
{
	return &compundShape_;
}