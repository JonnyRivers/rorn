#pragma once

#include "BulletBoxPrimitive.h"

using namespace Rorn::Engine;

BulletBoxPrimitive::BulletBoxPrimitive(float mass, float width, float length, float height) :
	BulletBounds(mass), 
	boxShape_(btVector3(width * 0.5f, height * 0.5, length * 0.5))
{
	
}

BulletBoxPrimitive::~BulletBoxPrimitive()
{
	
}

/*virtual*/ btCollisionShape* BulletBoxPrimitive::GetCollisionShape()
{
	return &boxShape_;
}