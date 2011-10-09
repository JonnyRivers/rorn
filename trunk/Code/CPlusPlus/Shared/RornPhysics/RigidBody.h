#pragma once

#include <vector>

#include "../RornMaths/Matrix4x4.h"
#include "../RornMaths/Quaternion.h"
#include "../RornMaths/Vector4.h"

namespace Rorn
{
	namespace Physics
	{
		class RigidBody
		{
		public:
			RigidBody(const Maths::Vector4& initialPosition, const Maths::Quaternion& initialOrientaton, float mass, const Maths::Matrix4x4& inertiaTensor);
		private:
			Maths::Vector4 position_;
			Maths::Quaternion orientation_;
			float mass_;
			Maths::Matrix4x4 inertiaTensor_;

			std::vector<Maths::Vector4> forces_;
		};
	}
}