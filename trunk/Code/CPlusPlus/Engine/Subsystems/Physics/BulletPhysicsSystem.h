#pragma once

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IPhysicsSystem.h"
#include "../../Interfaces/IDiagnostics.h"

class btDefaultCollisionConfiguration;

namespace Rorn
{
	namespace Engine
	{
		class BulletPhysicsSystem : public IPhysicsSystem
		{
		public:
			BulletPhysicsSystem(IDiagnostics* diagnostics);
			~BulletPhysicsSystem();

			
		private:
			BulletPhysicsSystem(BulletPhysicsSystem&);
			BulletPhysicsSystem& operator=(BulletPhysicsSystem&);

			IDiagnostics* diagnostics_;

			btDefaultCollisionConfiguration* m_collisionConfiguration;
		};
	}
}