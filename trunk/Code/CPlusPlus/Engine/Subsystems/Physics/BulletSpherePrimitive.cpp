#pragma once

#include "BulletSpherePrimitive.h"

using namespace Rorn::Engine;

BulletSpherePrimitive::BulletSpherePrimitive(float mass, float radius) :
	BulletBounds(mass), 
	sphereShape_(radius)
{
	
}

BulletSpherePrimitive::~BulletSpherePrimitive()
{
	
}

/*virtual*/ btCollisionShape* BulletSpherePrimitive::GetCollisionShape()
{
	return &sphereShape_;
}