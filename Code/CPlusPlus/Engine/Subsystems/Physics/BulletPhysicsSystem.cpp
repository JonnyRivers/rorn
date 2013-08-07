#pragma once

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "../../Exceptions/initialisation_exception.h"

#include "BulletPhysicsSystem.h"

using namespace Rorn::Engine;

BulletPhysicsSystem::BulletPhysicsSystem(IDiagnostics* diagnostics) : diagnostics_(diagnostics)
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
}

BulletPhysicsSystem::~BulletPhysicsSystem()
{
	delete m_collisionConfiguration;
}
