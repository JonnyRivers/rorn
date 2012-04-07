#include "RigidBody.h"

using namespace Rorn::Maths;
using namespace Rorn::Physics;

RigidBody::RigidBody()
{
}

RigidBody::RigidBody(const Vector4& initialPosition, const Quaternion& initialOrientation, float mass, const Matrix4x4& inertiaTensor)
	: position_(initialPosition), orientation_(initialOrientation), mass_(mass), inertiaTensor_(inertiaTensor),
	  linearVelocity_(0.0f, 0.0f, 0.0f, 0.0f), angularVelocity_(0.0f, 0.0f, 0.0f, 0.0f), accumulatedForce_(0.0f, 0.0f, 0.0f, 0.0f), accumulatedTorque_(0.0f, 0.0f, 0.0f, 0.0f)
{
}

void RigidBody::AddForce(const Vector4& force, const Vector4& forcePositionLocal)
{
	accumulatedForce_ += force;

	Vector4 torque = Vector4::CrossProduct(forcePositionLocal, force);
	accumulatedTorque_ += torque;
}

void RigidBody::Step(float timePassed)
{
	Vector4 linearAcceleration = accumulatedForce_ * (1.0f / mass_);
	linearVelocity_ += (linearAcceleration * timePassed);
	position_ += (linearVelocity_ * timePassed);

	Vector4 angularAcceleration = accumulatedTorque_ * inertiaTensor_;
	angularVelocity_ += (angularAcceleration * timePassed);
	orientation_.AddScaledVector(angularVelocity_, timePassed);
	orientation_ = Quaternion::Normalise(orientation_);

	accumulatedForce_ = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	accumulatedTorque_ = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

Matrix4x4 RigidBody::GetInstanceToWorldMatrix() const
{
	return Matrix4x4(
		1.0f - (2.0f * orientation_.Y * orientation_.Y + 2.0f * orientation_.Z * orientation_.Z),
		2.0f * orientation_.X * orientation_.Y - 2.0f * orientation_.W * orientation_.Z,
		2.0f * orientation_.X * orientation_.Z + 2.0f * orientation_.Y * orientation_.W,
		0.0f,
		2.0f * orientation_.X * orientation_.Y + 2.0f * orientation_.W * orientation_.Z,
		1.0f - (2.0f * orientation_.X * orientation_.X + 2.0f * orientation_.Z * orientation_.Z),
		2.0f * orientation_.Y * orientation_.Z - 2.0f * orientation_.W * orientation_.X,
		0.0f,
		2.0f * orientation_.X * orientation_.Z - 2.0f * orientation_.W * orientation_.Y,
		2.0f * orientation_.Y * orientation_.Z + 2.0f * orientation_.W * orientation_.X,
		1.0f - (2.0f * orientation_.X * orientation_.X + 2.0f * orientation_.Y * orientation_.Y),
		0.0f,
		position_.X,
		position_.Y,
		position_.Z,
		1.0f);
}