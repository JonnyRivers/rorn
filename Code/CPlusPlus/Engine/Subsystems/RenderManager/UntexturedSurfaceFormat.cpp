#include "UntexturedSurfaceFormat.h"

#include "ShaderCompiler.h"

using namespace Rorn::Engine;

/*static*/ UntexturedSurfaceFormat& UntexturedSurfaceFormat::instance_ = UntexturedSurfaceFormat();// init static instance

/*static*/ UntexturedSurfaceFormat& UntexturedSurfaceFormat::GetInstance()
{
	return instance_;
}

UntexturedSurfaceFormat::UntexturedSurfaceFormat()
	: vertexShader_(NULL), pixelShader_(NULL), vertexLayout_(NULL), constantBuffer_(NULL)
{
}

/*virtual*/ HRESULT UntexturedSurfaceFormat::Initialize(ID3D11Device* device)
{
	// Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    HRESULT hr = ShaderCompiler::CompileShaderFromFile( L"Untextured.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the vertex shader
	hr = device->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &vertexShader_ );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();
        return hr;
	}

	// Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	hr = device->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &vertexLayout_ );
	if( FAILED( hr ) )
        return hr;

	pVSBlob->Release();

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = ShaderCompiler::CompileShaderFromFile( L"Untextured.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK );
        return hr;
    }

	// Create the pixel shader
	hr = device->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pixelShader_ );
	pPSBlob->Release();
    if( FAILED( hr ) )
        return hr;

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UntexturedSurfaceFormat::ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = device->CreateBuffer( &bd, NULL, &constantBuffer_ );
    if( FAILED( hr ) )
        return hr;

	return S_OK;
}

/*virtual*/ void UntexturedSurfaceFormat::Release()
{
	if (constantBuffer_ != NULL)
		constantBuffer_->Release();

	if (pixelShader_ != NULL)
		pixelShader_->Release();

	if (vertexLayout_ != NULL)
		vertexLayout_->Release();

	if (vertexShader_ != NULL)
		vertexShader_->Release();
}

/*virtual*/ void UntexturedSurfaceFormat::SetupGPU(
	ID3D11DeviceContext* deviceContext,
	CXMMATRIX instanceToWorldMatrix, 
	CXMMATRIX worldToProjectionMatrix,
	const XMFLOAT4& ambientColor,
	const XMFLOAT4& diffuseColor,
	const XMFLOAT4& specularColor)
{
    ConstantBuffer constantBuffer;
	constantBuffer.ModelToWorldMatrix = XMMatrixTranspose( instanceToWorldMatrix );
	constantBuffer.WorldToProjectionMatrix = XMMatrixTranspose( worldToProjectionMatrix );
	constantBuffer.AmbientColor = ambientColor;
	constantBuffer.DiffuseColor = diffuseColor;
	constantBuffer.SpecularColor = specularColor;
	// lighting is hard coded for now
	constantBuffer.LightDir.x = 0.0f;
	constantBuffer.LightDir.y = 0.7071f;
	constantBuffer.LightDir.z = -0.7071f;
	constantBuffer.LightDir.w = 0.0f;
	constantBuffer.LightColor.x = 0.7f;
	constantBuffer.LightColor.y = 0.7f;
	constantBuffer.LightColor.z = 0.7f;
	constantBuffer.LightColor.w = 1.0f;
	deviceContext->UpdateSubresource( constantBuffer_, 0, NULL, &constantBuffer, 0, 0 );

    deviceContext->IASetInputLayout( vertexLayout_ );
	deviceContext->VSSetShader( vertexShader_, NULL, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &constantBuffer_ );
	deviceContext->PSSetShader( pixelShader_, NULL, 0 );
	deviceContext->PSSetConstantBuffers( 0, 1, &constantBuffer_ );
}
