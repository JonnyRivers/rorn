#pragma once

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "BulletBounds.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletBoxPrimitive : public BulletBounds
		{
		public:
			BulletBoxPrimitive(float mass, float width, float length, float height);
			~BulletBoxPrimitive();

			virtual btCollisionShape* GetCollisionShape();
			
		private:
			BulletBoxPrimitive(BulletBoxPrimitive&);
			BulletBoxPrimitive& operator=(BulletBoxPrimitive&);

			btBoxShape boxShape_;
		};
	}
}