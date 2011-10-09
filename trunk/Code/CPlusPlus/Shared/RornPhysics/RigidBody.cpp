#include "RigidBody.h"

using namespace Rorn::Maths;
using namespace Rorn::Physics;

RigidBody::RigidBody(const Vector4& initialPosition, const Quaternion& initialOrientation, float mass, const Matrix4x4& inertiaTensor)
	: position_(initialPosition), orientation_(initialOrientation), mass_(mass), inertiaTensor_(inertiaTensor)
{
}

