#pragma once

#include <vector>

#include "../../../../3rdParty/bullet-2.81-rev2613/src/btBulletDynamicsCommon.h"

#include "../../../Shared/RornMaths/Matrix4x4.h"

#include "BulletBounds.h"

namespace Rorn
{
	namespace Engine
	{
		class BulletCompositeBound : public BulletBounds
		{
		public:
			BulletCompositeBound();
			~BulletCompositeBound();

			void AddBox(const Maths::Matrix4x4& childToParentTransform, float mass, float width, float length, float height);

			virtual btCollisionShape* GetCollisionShape();
			
		private:
			BulletCompositeBound(BulletCompositeBound&);
			BulletCompositeBound& operator=(BulletCompositeBound&);

			btCompoundShape compundShape_;
			std::vector<std::unique_ptr<btCollisionShape>> childShapes_;
		};
	}
}