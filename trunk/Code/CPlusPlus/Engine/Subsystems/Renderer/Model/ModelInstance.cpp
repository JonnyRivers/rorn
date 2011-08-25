#pragma once

#include "ModelInstance.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

ModelInstance::ModelInstance(unsigned int modelId, const Rorn::Maths::Matrix4x4& instanceToWorldMatrix)
	: modelId_(modelId), instanceToWorldMatrix_(instanceToWorldMatrix)
{
}

ModelInstance::~ModelInstance()
{
}

void ModelInstance::RotateY(float angle)
{
	Matrix4x4 rotationMatrix = Matrix4x4::BuildYRotationMatrix(angle);
	instanceToWorldMatrix_ = rotationMatrix * instanceToWorldMatrix_;
}
