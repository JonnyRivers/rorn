#include "RenderCommand.h"

#include "SurfaceFormat.h"

using namespace Rorn::Engine;

RenderCommand::RenderCommand(const SurfaceFormat* surfaceFormat)
	: surfaceFormat_(surfaceFormat), primitiveType_(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

RenderCommand::~RenderCommand(void)
{
}

void RenderCommand::Draw(ID3D11DeviceContext* deviceContext, CXMMATRIX instanceToWorldMatrix, CXMMATRIX worldToProjectionMatrix) const
{
	surfaceFormat_->SetupGPU(deviceContext, instanceToWorldMatrix, worldToProjectionMatrix);

	deviceContext->IASetPrimitiveTopology( primitiveType_ );
	//g_pImmediateContext->DrawIndexed( 36, 0, 0 );
}