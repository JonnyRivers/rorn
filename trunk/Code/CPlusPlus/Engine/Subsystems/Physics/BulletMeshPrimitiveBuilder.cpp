#pragma once

#include "BulletMeshPrimitiveBuilder.h"

using namespace Rorn::Engine;

BulletMeshPrimitiveBuilder::BulletMeshPrimitiveBuilder(float mass) :
	mass_(mass), triangleMesh_(new btTriangleMesh)
{
	
}

BulletMeshPrimitiveBuilder::~BulletMeshPrimitiveBuilder()
{
	if(triangleMesh_ != 0)
		delete triangleMesh_;
}

void BulletMeshPrimitiveBuilder::AddTriangle(float vert0X, float vert0Y, float vert0Z,
											 float vert1X, float vert1Y, float vert1Z,
											 float vert2X, float vert2Y, float vert2Z)
{
	triangleMesh_->addTriangle(btVector3(vert0X, vert0Y, vert0Z), btVector3(vert1X, vert1Y, vert1Z), btVector3(vert2X, vert2Y, vert2Z), false);
}

BulletMeshPrimitive* BulletMeshPrimitiveBuilder::ToMeshPrimitive()
{
	BulletMeshPrimitive* meshPrimitive = new BulletMeshPrimitive(mass_, triangleMesh_);
	triangleMesh_ = NULL;// the primitive takes ownership of the triangle mesh

	return meshPrimitive;
}