#include "UntexturedSurfaceFormat.h"

#include "ShaderCompiler.h"

using namespace Rorn::Engine;

UntexturedSurfaceFormat::UntexturedSurfaceFormat()
	: vertexShader_(NULL), pixelShader_(NULL), vertexLayout_(NULL), constantBuffer_(NULL)
{
}

/*virtual*/ HRESULT UntexturedSurfaceFormat::Startup(ID3D11Device* device)
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
        { "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

/*virtual*/ void UntexturedSurfaceFormat::Shutdown()
{
	if (constantBuffer_)
		constantBuffer_->Release();

	if (pixelShader_)
		pixelShader_->Release();

	if (vertexLayout_)
		vertexLayout_->Release();

	if (vertexShader_)
		vertexShader_->Release();
}

/*virtual*/ void UntexturedSurfaceFormat::SetupGPU(ID3D11DeviceContext* deviceContext,
	CXMMATRIX instanceToWorldMatrix, CXMMATRIX worldToProjectionMatrix) const
{
    ConstantBuffer constantBuffer;
	constantBuffer.ModelToWorldMatrix = XMMatrixTranspose( instanceToWorldMatrix );
	constantBuffer.WorldToProjectionMatrix = XMMatrixTranspose( worldToProjectionMatrix );
	deviceContext->UpdateSubresource( constantBuffer_, 0, NULL, &constantBuffer, 0, 0 );

    deviceContext->IASetInputLayout( vertexLayout_ );
	deviceContext->VSSetShader( vertexShader_, NULL, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &constantBuffer_ );
	deviceContext->PSSetShader( pixelShader_, NULL, 0 );
}
