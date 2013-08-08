#pragma once

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "BulletBounds.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletSpherePrimitive : public BulletBounds
		{
		public:
			BulletSpherePrimitive(float mass, float radius);
			~BulletSpherePrimitive();

			virtual btCollisionShape* GetCollisionShape();
			
		private:
			BulletSpherePrimitive(BulletSpherePrimitive&);
			BulletSpherePrimitive& operator=(BulletSpherePrimitive&);

			btSphereShape sphereShape_;
		};
	}
}