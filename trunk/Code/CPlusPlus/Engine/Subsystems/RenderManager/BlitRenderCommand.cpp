#include "BlitRenderCommand.h"

#include <cassert>

#include "../TextureManager/TextureManager.h"

#include "BlitSurfaceFormat.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

BlitRenderCommand::BlitRenderCommand(ID3D11Device* device, int textureId)
	: textureId_(textureId), 
	  vertexCount_(0), 
	  vertexDataSize_(0), 
	  vertexBuffer_(NULL), 
	  indexCount_(0), 
	  indexDataSize_(0), 
	  indexBuffer_(NULL)
{
	// Zero vertex data
	vertexCount_ = 1024;
	vertexDataSize_ = vertexCount_ * sizeof(BlitRenderCommand::VertexFormat);
	BlitRenderCommand::VertexFormat* vertices = new BlitRenderCommand::VertexFormat[vertexCount_];
	memset(vertices, 0, vertexDataSize_);

	// Create buffer
	D3D11_BUFFER_DESC vertexBufferDescription;
	ZeroMemory( &vertexBufferDescription, sizeof(vertexBufferDescription) );
    vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDescription.ByteWidth = vertexDataSize_;
    vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA vertexResourceData;
	ZeroMemory( &vertexResourceData, sizeof(vertexResourceData) );
    vertexResourceData.pSysMem = vertices;
    HRESULT hr = device->CreateBuffer( &vertexBufferDescription, &vertexResourceData, &vertexBuffer_ );
	assert( SUCCEEDED( hr ) );
	delete [] vertices;

	// Read index data
	indexCount_ = 1024;
	indexDataSize_ = indexCount_ * sizeof(unsigned int);
	unsigned int* indices = new unsigned int[indexCount_];
	memset(indices, 0, indexDataSize_);

	D3D11_BUFFER_DESC indexBufferDescription;
	ZeroMemory( &indexBufferDescription, sizeof(indexBufferDescription) );
	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDescription.ByteWidth = indexDataSize_;
    indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA indexResourceData;
	ZeroMemory( &indexResourceData, sizeof(indexResourceData) );
    indexResourceData.pSysMem = indices;
    hr = device->CreateBuffer( &indexBufferDescription, &indexResourceData, &indexBuffer_ );
	assert( SUCCEEDED( hr ) );
	delete [] indices;

	vertexCount_ = 0;
	indexCount_ = 0;
}

void BlitRenderCommand::UpdateData(ID3D11DeviceContext* deviceContext, unsigned int vertexCount, const VertexFormat* vertexData, unsigned int indexCount, const int* indexData)
{
	vertexCount_ = vertexCount;
	deviceContext->UpdateSubresource( vertexBuffer_, 0, NULL, vertexData, 0, 0 );
	indexCount_ = indexCount;
	deviceContext->UpdateSubresource( indexBuffer_, 0, NULL, indexData, 0, 0 );
}

/*virtual*/ void BlitRenderCommand::Draw(ID3D11DeviceContext* deviceContext)
{
	// include everything required by this surface type (including lights)
	BlitSurfaceFormat::GetInstance().SetupGPU(deviceContext);

	// draw the triangles owned by the command
	UINT stride = sizeof( BlitRenderCommand::VertexFormat );
    UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	deviceContext->IASetIndexBuffer( indexBuffer_, DXGI_FORMAT_R32_UINT, 0 );
    deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	ID3D11ShaderResourceView* diffuseTexture = TextureManager::GetInstance().GetTexture(textureId_);
	deviceContext->PSSetShaderResources( 0, 1, &diffuseTexture);
	deviceContext->DrawIndexed(indexCount_, 0, 0);
}

/*virtual*/ void BlitRenderCommand::Release()
{
	if(vertexBuffer_ != NULL)
		vertexBuffer_->Release();

	if(indexBuffer_ != NULL)
		indexBuffer_->Release();
}
