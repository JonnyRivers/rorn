#include "ModelInstance.h"

#include "Model.h"

using namespace Rorn::Engine;

ModelInstance::ModelInstance(const Model& model, CXMMATRIX instanceToWorldMatrix)
	: model_(model), instanceToWorldMatrix_(instanceToWorldMatrix)
{
}

ModelInstance::~ModelInstance(void)
{
}

void ModelInstance::Draw(ID3D11DeviceContext* deviceContext, CXMMATRIX worldToProjectionMatrix) const
{
	model_.Draw(deviceContext, instanceToWorldMatrix_, worldToProjectionMatrix);
}
