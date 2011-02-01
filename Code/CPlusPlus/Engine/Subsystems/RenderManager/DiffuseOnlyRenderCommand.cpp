#include "DiffuseOnlyRenderCommand.h"

#include "../TextureManager/TextureManager.h"

#include "DiffuseOnlySurfaceFormat.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

DiffuseOnlyRenderCommand::DiffuseOnlyRenderCommand(void)
	: compiledTextureId_(0), 
	  vertexCount_(0), 
	  vertexDataSize_(0), 
	  vertexBuffer_(NULL), 
	  indexCount_(0), 
	  indexDataSize_(0), 
	  indexBuffer_(NULL)
{
}

HRESULT DiffuseOnlyRenderCommand::LoadFromFile(FileReader& fileReader, ID3D11Device* device, const std::map<int, int>& textureIdMap)
{
	int primitveTypeValue = fileReader.ReadInt();
	RenderCommand::PrimitiveType primitiveType = static_cast<RenderCommand::PrimitiveType>(primitveTypeValue);

	// Read color data
	fileReader.ReadData(&ambientColor_, sizeof(ambientColor_));
	fileReader.ReadData(&diffuseColor_, sizeof(diffuseColor_));
	fileReader.ReadData(&specularColor_, sizeof(specularColor_));
	phongExponent_ = fileReader.ReadFloat();

	int localTextureIndex = fileReader.ReadInt();
	compiledTextureId_ = textureIdMap.find(localTextureIndex)->second;

	// Read vertex data
	vertexCount_ = fileReader.ReadInt();
	vertexDataSize_ = vertexCount_ * sizeof(DiffuseOnlyRenderCommand::VertexFormat);
	DiffuseOnlyRenderCommand::VertexFormat* vertices = new DiffuseOnlyRenderCommand::VertexFormat[vertexCount_];
	fileReader.ReadData(vertices, vertexDataSize_);

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

	delete [] vertices;

    if( FAILED( hr ) )
        return hr;

	// Read index data
	indexCount_ = fileReader.ReadInt();
	indexDataSize_ = indexCount_ * sizeof(int);
	int* indices = new int[indexCount_];
	fileReader.ReadData(indices, indexDataSize_);

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

	delete [] indices;

    if( FAILED( hr ) )
        return hr;

	return S_OK;
}

/*virtual*/ void DiffuseOnlyRenderCommand::Draw(ID3D11DeviceContext* deviceContext, 
	const Matrix4x4& instanceToWorldMatrix, const Matrix4x4& worldToProjectionMatrix)
{
	// include everything required by this surface type (including lights)
	DiffuseOnlySurfaceFormat::GetInstance().SetupGPU(deviceContext, instanceToWorldMatrix, worldToProjectionMatrix,
		ambientColor_, diffuseColor_, specularColor_, phongExponent_);

	// draw the triangles owned by the command
	UINT stride = sizeof( DiffuseOnlyRenderCommand::VertexFormat );
    UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	deviceContext->IASetIndexBuffer( indexBuffer_, DXGI_FORMAT_R32_UINT, 0 );
    deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	ID3D11ShaderResourceView* diffuseTexture = TextureManager::GetInstance().GetTexture(compiledTextureId_);
	deviceContext->PSSetShaderResources( 0, 1, &diffuseTexture);
	deviceContext->DrawIndexed(indexCount_, 0, 0);
}

/*virtual*/ void DiffuseOnlyRenderCommand::Release()
{
	if(vertexBuffer_ != NULL)
		vertexBuffer_->Release();

	if(indexBuffer_ != NULL)
		indexBuffer_->Release();
}
