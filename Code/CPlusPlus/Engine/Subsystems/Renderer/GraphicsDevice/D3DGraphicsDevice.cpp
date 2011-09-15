#pragma once

#include <cassert>

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../Material/VertexFormatFlags.h"

#include "D3DGraphicsDevice.h"

using namespace Rorn::Engine;

D3DGraphicsDevice::D3DGraphicsDevice(HWND applicationWindowHandle, IDiagnostics* diagnostics) 
	: diagnostics_(diagnostics), 
	  outputDimensions_(applicationWindowHandle), 
	  device_(diagnostics, applicationWindowHandle, outputDimensions_.Width, outputDimensions_.Height),
	  renderTargetView_(diagnostics, device_.Device, device_.SwapChain),
	  depthStencilTexture_(diagnostics, device_.Device, outputDimensions_.Width, outputDimensions_.Height),
	  depthStencilView_(diagnostics, device_.Device, device_.DeviceContext, renderTargetView_.RenderTargetView, depthStencilTexture_.Texture2D),
	  blendState_(diagnostics, device_.Device, device_.DeviceContext),
	  viewport_(diagnostics, device_.DeviceContext, outputDimensions_.Width, outputDimensions_.Height)
{
	diagnostics_->GetLoggingStream() << "D3DGraphicsDevice instance was created successfully." << std::endl;
}

D3DGraphicsDevice::~D3DGraphicsDevice()
{
	diagnostics_->GetLoggingStream() << "D3DGraphicsDevice instance is being destroyed." << std::endl;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateConstantBuffer(unsigned int dataSize)
{
	unsigned int newId = static_cast<unsigned int>(constantBuffers_.size());
	constantBuffers_[newId] = std::unique_ptr<D3D11Buffer>(new D3D11Buffer(diagnostics_, device_.Device, D3D11_BIND_CONSTANT_BUFFER, dataSize));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateIndexBuffer(const void* data, unsigned int dataSize)
{
	unsigned int newId = static_cast<unsigned int>(indexBuffers_.size());
	indexBuffers_[newId] = std::unique_ptr<D3D11Buffer>(new D3D11Buffer(diagnostics_, device_.Device, D3D11_BIND_INDEX_BUFFER, dataSize, data));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreatePixelShader(const wchar_t* shaderFilename)
{
	unsigned int newId = static_cast<unsigned int>(pixelShaders_.size());
	pixelShaders_[newId] = std::unique_ptr<D3D11PixelShader>(new D3D11PixelShader(diagnostics_, device_.Device, shaderFilename));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateSamplerState()
{
	unsigned int newId = static_cast<unsigned int>(samplerStates_.size());
	samplerStates_[newId] = std::unique_ptr<D3D11SamplerState>(new D3D11SamplerState(diagnostics_, device_.Device));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateTexture(const void* data, unsigned int dataSize)
{
	unsigned int newId = static_cast<unsigned int>(textures_.size());
	textures_[newId] = std::unique_ptr<D3D11ShaderResourceView>(new D3D11ShaderResourceView(diagnostics_, device_.Device, dataSize, data));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateVertexBuffer(const void* data, unsigned int dataSize)
{
	unsigned int newId = static_cast<unsigned int>(vertexBuffers_.size());
	vertexBuffers_[newId] = std::unique_ptr<D3D11Buffer>(new D3D11Buffer(diagnostics_, device_.Device, D3D11_BIND_VERTEX_BUFFER, dataSize, data));

	return newId;
}

/*virtual*/ unsigned int D3DGraphicsDevice::CreateVertexShader(const wchar_t* shaderFilename, unsigned int vertexFormatFlags)
{
	unsigned int newId = static_cast<unsigned int>(vertexShaders_.size());
	vertexShaders_[newId] = std::unique_ptr<D3D11VertexShader>(new D3D11VertexShader(diagnostics_, device_.Device, shaderFilename, vertexFormatFlags));

	return newId;
}

/*virtual*/ void D3DGraphicsDevice::UpdateConstantBufferData(unsigned int constantBufferId, const void* data)
{
	std::map<unsigned int, std::unique_ptr<D3D11Buffer>>::const_iterator constantBufferIter = constantBuffers_.find(constantBufferId);
	assert(constantBufferIter != constantBuffers_.end());

	device_.DeviceContext->UpdateSubresource( constantBufferIter->second->Buffer, 0, NULL, data, 0, 0 );
}

/*virtual*/ void D3DGraphicsDevice::UpdateIndexBufferData(unsigned int indexBufferId, const void* data)
{
	std::map<unsigned int, std::unique_ptr<D3D11Buffer>>::const_iterator indexBufferIter = indexBuffers_.find(indexBufferId);
	assert(indexBufferIter != indexBuffers_.end());

	device_.DeviceContext->UpdateSubresource( indexBufferIter->second->Buffer, 0, NULL, data, 0, 0 );
}

/*virtual*/ void D3DGraphicsDevice::UpdateVertexBufferData(unsigned int vertexBufferId, const void* data)
{
	std::map<unsigned int, std::unique_ptr<D3D11Buffer>>::const_iterator vertexBufferIter = vertexBuffers_.find(vertexBufferId);
	assert(vertexBufferIter != vertexBuffers_.end());

	device_.DeviceContext->UpdateSubresource( vertexBufferIter->second->Buffer, 0, NULL, data, 0, 0 );
}

/*virtual*/ void D3DGraphicsDevice::SetVertexShader(unsigned int vertexShaderId)
{
	std::map<unsigned int, std::unique_ptr<D3D11VertexShader>>::const_iterator vertexShaderIter = vertexShaders_.find(vertexShaderId);
	assert(vertexShaderIter != vertexShaders_.end());

	device_.DeviceContext->IASetInputLayout( vertexShaderIter->second->VertexLayout );
	device_.DeviceContext->VSSetShader( vertexShaderIter->second->VertexShader, NULL, 0 );
}

/*virtual*/ void D3DGraphicsDevice::SetVertexShaderConstantBuffer(unsigned int constantBufferId)
{
	std::map<unsigned int, std::unique_ptr<D3D11Buffer>>::const_iterator constantBufferIter = constantBuffers_.find(constantBufferId);
	assert(constantBufferIter != constantBuffers_.end());

	device_.DeviceContext->VSSetConstantBuffers( 0, 1, &constantBufferIter->second->Buffer );
}

/*virtual*/ void D3DGraphicsDevice::SetPixelShader(unsigned int pixelShaderId)
{
	std::map<unsigned int, std::unique_ptr<D3D11PixelShader>>::const_iterator pixelShaderIter = pixelShaders_.find(pixelShaderId);
	assert(pixelShaderIter != pixelShaders_.end());

	device_.DeviceContext->PSSetShader( pixelShaderIter->second->PixelShader, NULL, 0 );
}

/*virtual*/ void D3DGraphicsDevice::SetPixelShaderConstantBuffer(unsigned int constantBufferId)
{
	std::map<unsigned int, std::unique_ptr<D3D11Buffer>>::const_iterator constantBufferIter = constantBuffers_.find(constantBufferId);
	assert(constantBufferIter != constantBuffers_.end());

	device_.DeviceContext->PSSetConstantBuffers( 0, 1, &constantBufferIter->second->Buffer );
}

/*virtual*/ void D3DGraphicsDevice::SetSamplerState(unsigned int samplerStateId)
{
	std::map<unsigned int, std::unique_ptr<D3D11SamplerState>>::const_iterator samplerStateIter = samplerStates_.find(samplerStateId);
	assert(samplerStateIter != samplerStates_.end());

	device_.DeviceContext->PSSetSamplers( 0, 1, &samplerStateIter->second->SamplerState );
}

/*virtual*/ void D3DGraphicsDevice::SetVertexBuffer(unsigned int vertexBufferId, unsigned int vertexStride)
{
	std::map<unsigned int, std::unique_ptr<D3D11Buffer>>::const_iterator vertexBufferIter = vertexBuffers_.find(vertexBufferId);
	assert(vertexBufferIter != vertexBuffers_.end());

	UINT offset = 0;
	device_.DeviceContext->IASetVertexBuffers(0, 1, &vertexBufferIter->second->Buffer, &vertexStride, &offset);
}

/*virtual*/ void D3DGraphicsDevice::SetIndexBuffer(unsigned int indexBufferId)
{
	std::map<unsigned int, std::unique_ptr<D3D11Buffer>>::const_iterator indexBufferIter = indexBuffers_.find(indexBufferId);
	assert(indexBufferIter != indexBuffers_.end());

	device_.DeviceContext->IASetIndexBuffer( indexBufferIter->second->Buffer, DXGI_FORMAT_R32_UINT, 0 );
}

/*virtual*/ void D3DGraphicsDevice::SetTexture(unsigned int textureId)
{
	std::map<unsigned int, std::unique_ptr<D3D11ShaderResourceView>>::const_iterator textureIter = textures_.find(textureId);
	assert(textureIter != textures_.end());

	device_.DeviceContext->PSSetShaderResources( 0, 1, &textureIter->second->ShaderResourceView);
}

/*virtual*/ unsigned int D3DGraphicsDevice::GetOutputWidth() const
{
	return outputDimensions_.Width;
}

/*virtual*/ unsigned int D3DGraphicsDevice::GetOutputHeight() const
{
	return outputDimensions_.Height;
}

/*virtual*/ float D3DGraphicsDevice::GetOutputAspectRatio() const
{
	return outputDimensions_.AspectRatio;
}

/*virtual*/ void D3DGraphicsDevice::ClearView(const Rorn::Maths::Float4& backColour)
{
	float clearColor[4] = { backColour.X, backColour.Y, backColour.Z, backColour.W };
	device_.DeviceContext->ClearRenderTargetView( renderTargetView_.RenderTargetView, clearColor );
	device_.DeviceContext->ClearDepthStencilView( depthStencilView_.DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

/*virtual*/ void D3DGraphicsDevice::DrawIndexed(unsigned int numIndices)
{
	device_.DeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	device_.DeviceContext->DrawIndexed(numIndices, 0, 0);
}

/*virtual*/ void D3DGraphicsDevice::Present()
{
	device_.SwapChain->Present( 0, 0 );
}