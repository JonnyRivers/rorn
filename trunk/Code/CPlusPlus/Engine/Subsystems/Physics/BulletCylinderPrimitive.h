#pragma once

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "BulletBounds.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletCylinderPrimitive : public BulletBounds
		{
		public:
			BulletCylinderPrimitive(float mass, float radius, float height);
			~BulletCylinderPrimitive();

			virtual btCollisionShape* GetCollisionShape();
			
		private:
			BulletCylinderPrimitive(BulletCylinderPrimitive&);
			BulletCylinderPrimitive& operator=(BulletCylinderPrimitive&);

			btCylinderShape cylinderShape_;
		};
	}
}