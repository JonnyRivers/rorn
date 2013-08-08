#pragma once

#include "ModelInstance.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

ModelInstance::ModelInstance(unsigned int modelId, IPhysicsSystem* physicsSystem, unsigned int boundsInstanceId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
	: modelId_(modelId), hasBounds_(true), physicsSystem_(physicsSystem), boundsInstanceId_(boundsInstanceId)
{
}

ModelInstance::ModelInstance(unsigned int modelId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
	: modelId_(modelId), instanceToWorldMatrix_(instanceToWorldMatrix), hasBounds_(false)
{
}

ModelInstance::~ModelInstance()
{
}

const Matrix4x4& ModelInstance::GetInstanceToWorldMatrix()
{
	if(hasBounds_)
	{
		physicsSystem_->GetBoundsInstanceToWorldTransform(boundsInstanceId_, instanceToWorldMatrix_);
	}
	
	return instanceToWorldMatrix_;
}

/*virtual*/ void ModelInstance::SetLinearVelocity(const Vector4& linearVelocity)
{
	if(hasBounds_)
	{
		physicsSystem_->SetBoundsInstanceLinearVelocity(boundsInstanceId_, linearVelocity);
	}
}

/*virtual*/ void ModelInstance::SetInstanceToWorldMatrix(const Matrix4x4& instanceToWorldMatrix)
{
	if(hasBounds_)
	{
		physicsSystem_->SetBoundsInstanceToWorldTransform(boundsInstanceId_, instanceToWorldMatrix);
	}

	instanceToWorldMatrix_ = instanceToWorldMatrix;
}