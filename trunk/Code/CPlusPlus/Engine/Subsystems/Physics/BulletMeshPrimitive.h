#pragma once

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "BulletBounds.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletMeshPrimitive : public BulletBounds
		{
		public:
			friend class BulletMeshPrimitiveBuilder;
			
			~BulletMeshPrimitive();

			virtual btCollisionShape* GetCollisionShape();
			
		private:
			BulletMeshPrimitive(float mass, btTriangleMesh* triangleMesh);

			BulletMeshPrimitive(BulletMeshPrimitive&);
			BulletMeshPrimitive& operator=(BulletMeshPrimitive&);

			btTriangleMesh* triangleMesh_;
			btConvexTriangleMeshShape meshShape_;
		};
	}
}