#pragma once

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletBounds
		{
		public:
			BulletBounds(float mass);
			~BulletBounds();

			virtual btCollisionShape* GetCollisionShape() = 0;
			float GetMass() const;
		protected:
			float mass_;
			
		private:
			BulletBounds(BulletBounds&);
			BulletBounds& operator=(BulletBounds&);
		};
	}
}