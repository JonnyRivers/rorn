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

/*virtual*/ void ModelInstance::SetInstanceToWorldMatrix(const Matrix4x4& instanceToWorldMatrix)
{
	instanceToWorldMatrix_ = instanceToWorldMatrix;
}