#pragma once

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "BulletMeshPrimitive.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletMeshPrimitiveBuilder
		{
		public:
			BulletMeshPrimitiveBuilder(float mass_);
			~BulletMeshPrimitiveBuilder();

			void AddTriangle(
				float vert0X, float vert0Y, float vert0Z,
				float vert1X, float vert1Y, float vert1Z,
				float vert2X, float vert2Y, float vert2Z);

			BulletMeshPrimitive* ToMeshPrimitive();
		private:
			BulletMeshPrimitiveBuilder(BulletMeshPrimitiveBuilder&);
			BulletMeshPrimitiveBuilder& operator=(BulletMeshPrimitiveBuilder&);

			float mass_;
			btTriangleMesh* triangleMesh_;
		};
	}
}