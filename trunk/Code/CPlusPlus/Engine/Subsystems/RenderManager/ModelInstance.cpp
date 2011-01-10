#include "ModelInstance.h"

#include "Model.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

ModelInstance::ModelInstance(const Model* model, const Matrix4x4& instanceToWorldMatrix)
	: model_(model), instanceToWorldMatrix_(instanceToWorldMatrix)
{
}

ModelInstance::~ModelInstance(void)
{
}

void ModelInstance::Draw(ID3D11DeviceContext* deviceContext, const Matrix4x4& worldToProjectionMatrix) const
{
	model_->Draw(deviceContext, instanceToWorldMatrix_, worldToProjectionMatrix);
}

void ModelInstance::RotateY(float angle)
{
	Matrix4x4 rotationMatrix = Matrix4x4::BuildYRotationMatrix(angle);
	instanceToWorldMatrix_ *= rotationMatrix;
}