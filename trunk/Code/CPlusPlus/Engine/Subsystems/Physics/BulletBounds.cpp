#pragma once

#include "BulletBounds.h"

using namespace Rorn::Engine;

BulletBounds::BulletBounds(float mass) :
	mass_(mass)
{
	
}

BulletBounds::~BulletBounds()
{
	
}

float BulletBounds::GetMass() const
{
	return mass_;
}