#include "BlitSurfaceFormat.h"

#include "../DiagnosticsManager/DiagnosticsManager.h"

#include "RenderManager.h"
#include "ShaderCompiler.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

/*static*/ BlitSurfaceFormat& BlitSurfaceFormat::instance_ = BlitSurfaceFormat();// init static instance

/*static*/ BlitSurfaceFormat& BlitSurfaceFormat::GetInstance()
{
	return instance_;
}

BlitSurfaceFormat::BlitSurfaceFormat()
	: vertexShader_(NULL), pixelShader_(NULL), vertexLayout_(NULL), sampler_(NULL)
{
}

/*virtual*/ HRESULT BlitSurfaceFormat::Initialize(ID3D11Device* device)
{
	// Compile the vertex shader
    ID3DBlob* pVSBlob = NULL;
    HRESULT hr = ShaderCompiler::CompileShaderFromFile( L"Blit.fx", "VS", "vs_4_0", &pVSBlob );
    if( FAILED( hr ) )
	{
		return hr;
	}

	// Create the vertex shader
	hr = device->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &vertexShader_ );
	if( FAILED( hr ) )
	{	
		pVSBlob->Release();

		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of vertex shader");
        return hr;
	}

	// Define the input layout
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	hr = device->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
                                          pVSBlob->GetBufferSize(), &vertexLayout_ );
	if( FAILED( hr ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of vertex buffer input layout");
        return hr;
	}

	pVSBlob->Release();

	// Compile the pixel shader
	ID3DBlob* pPSBlob = NULL;
	hr = ShaderCompiler::CompileShaderFromFile( L"Blit.fx", "PS", "ps_4_0", &pPSBlob );
    if( FAILED( hr ) )
		return hr;

	// Create the pixel shader
	hr = device->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &pixelShader_ );
	pPSBlob->Release();
    if( FAILED( hr ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of pixel shader");
        return hr;
	}

	// Create the sample state
    D3D11_SAMPLER_DESC samplerDescription;
    ZeroMemory( &samplerDescription, sizeof(samplerDescription) );
    samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDescription.MinLOD = 0;
    samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;
    hr = device->CreateSamplerState( &samplerDescription, &sampler_ );
    if( FAILED( hr ) )
        return hr;

	return S_OK;
}

/*virtual*/ void BlitSurfaceFormat::Release()
{
	if (sampler_ != NULL)
		sampler_->Release();

	if (pixelShader_ != NULL)
		pixelShader_->Release();

	if (vertexLayout_ != NULL)
		vertexLayout_->Release();

	if (vertexShader_ != NULL)
		vertexShader_->Release();
}

/*virtual*/ void BlitSurfaceFormat::SetupGPU(
	ID3D11DeviceContext* deviceContext)
{
    deviceContext->IASetInputLayout( vertexLayout_ );
	deviceContext->VSSetShader( vertexShader_, NULL, 0 );
	deviceContext->PSSetShader( pixelShader_, NULL, 0 );
    deviceContext->PSSetSamplers( 0, 1, &sampler_ );
}
