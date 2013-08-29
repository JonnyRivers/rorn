#pragma once

#include <map>

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "../../Shared/RornMaths/Matrix4x4.h"

#include "../../Exceptions/initialisation_exception.h"

#include "../../Interfaces/IPhysicsSystem.h"
#include "../../Interfaces/IDiagnostics.h"

#include "BulletBounds.h"
#include "BulletBoundsInstance.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletPhysicsSystem : public IPhysicsSystem
		{
		public:
			BulletPhysicsSystem(IDiagnostics* diagnostics);
			~BulletPhysicsSystem();

			void Step(float timeElapsed);

			virtual void GetBoundsInstanceToWorldTransform(unsigned int boundsInstanceId, Maths::Matrix4x4& instanceToWorldMatrix) const;

			virtual void SetBoundsInstanceLinearVelocity(unsigned int boundsInstanceId, const Maths::Vector4& linearVelocity);
			virtual void SetBoundsInstanceToWorldTransform(unsigned int boundsInstanceId, const Maths::Matrix4x4& instanceToWorldMatrix);

			virtual bool GetNumActiveObjects() const;

			virtual unsigned int CreateBoundsInstance(unsigned int boundsId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix);
			virtual unsigned int LoadBounds(unsigned int numPhysicsPrimitives, StreamReader& fileReader);
			virtual void SetEnabled(bool enabled) { isEnabled_ = enabled; }
		private:
			BulletPhysicsSystem(BulletPhysicsSystem&);
			BulletPhysicsSystem& operator=(BulletPhysicsSystem&);

			IDiagnostics* diagnostics_;

			std::map<unsigned int, std::unique_ptr<BulletBounds>> bounds_;
			std::map<unsigned int, std::unique_ptr<BulletBoundsInstance>> boundsInstances_;
			bool isEnabled_;

			btDefaultCollisionConfiguration collisionConfiguration_;
			btCollisionDispatcher collisionDispatcher_;
			btDbvtBroadphase broadphase_;
			btSequentialImpulseConstraintSolver solver_;
			btDiscreteDynamicsWorld dynamicsWorld_;

			static const unsigned int nullPhysicsId_ = 0;
		};
	}
}