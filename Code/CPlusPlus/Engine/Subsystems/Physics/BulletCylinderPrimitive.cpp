#pragma once

#include "BulletCylinderPrimitive.h"

using namespace Rorn::Engine;

BulletCylinderPrimitive::BulletCylinderPrimitive(float mass, float radius, float height) :
	BulletBounds(mass), 
	cylinderShape_(btVector3(radius * 0.5f, height * 0.5, radius * 0.5))
{
	
}

BulletCylinderPrimitive::~BulletCylinderPrimitive()
{
	
}

/*virtual*/ btCollisionShape* BulletCylinderPrimitive::GetCollisionShape()
{
	return &cylinderShape_;
}