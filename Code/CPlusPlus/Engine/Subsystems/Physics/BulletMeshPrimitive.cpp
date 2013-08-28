#pragma once

#include "BulletMeshPrimitive.h"

using namespace Rorn::Engine;

BulletMeshPrimitive::BulletMeshPrimitive(float mass, btTriangleMesh* triangleMesh) :
	BulletBounds(mass), 
	triangleMesh_(triangleMesh),
	meshShape_(triangleMesh)
{
}

BulletMeshPrimitive::~BulletMeshPrimitive()
{
	delete triangleMesh_;
}

/*virtual*/ btCollisionShape* BulletMeshPrimitive::GetCollisionShape()
{
	return &meshShape_;
}