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
			RigidBody();
			RigidBody(const Maths::Vector4& initialPosition, const Maths::Quaternion& initialOrientaton, float mass, const Maths::Matrix4x4& inertiaTensor);

			void AddForce(const Maths::Vector4& force, const Maths::Vector4& forcePositionLocal);
			void Step(float timePassed);

			Maths::Matrix4x4 GetInstanceToWorldMatrix() const;

		private:
			Maths::Vector4 position_;
			Maths::Quaternion orientation_;
			float mass_;
			Maths::Matrix4x4 inertiaTensor_;

			Maths::Vector4 linearVelocity_;
			Maths::Vector4 angularVelocity_;

			Maths::Vector4 accumulatedForce_;
			Maths::Vector4 accumulatedTorque_;
		};
	}
}