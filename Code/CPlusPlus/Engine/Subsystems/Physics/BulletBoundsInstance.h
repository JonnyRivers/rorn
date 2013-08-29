#pragma once

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "../../../Shared/RornMaths/Matrix4x4.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletBoundsInstance
		{
		public:
			BulletBoundsInstance(btDiscreteDynamicsWorld& dynamicsWorld, float mass, const btTransform& instanceToWorldTransform, btCollisionShape* collisionShape, const btVector3& localInertia);
			~BulletBoundsInstance();

			void GetInstanceToWorldMatrix(Rorn::Maths::Matrix4x4& instanceToWorldMatrix) const;
			bool IsActive() const;

			void SetInstanceToWorldMatrix(const Rorn::Maths::Matrix4x4& instanceToWorldMatrix);
			void SetLinearVelocity(const Rorn::Maths::Vector4& linearVelocity);
		private:
			BulletBoundsInstance(BulletBoundsInstance&);
			BulletBoundsInstance& operator=(BulletBoundsInstance&);

			btDefaultMotionState motionState_;
			btRigidBody rigidBody_;
		};
	}
}