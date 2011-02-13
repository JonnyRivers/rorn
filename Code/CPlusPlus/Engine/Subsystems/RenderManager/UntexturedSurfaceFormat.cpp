#include "UntexturedSurfaceFormat.h"

#include "../DiagnosticsManager/DiagnosticsManager.h"

#include "../LightingManager/DirectionalLight.h"
#include "../LightingManager/LightingManager.h"

#include "RenderManager.h"
#include "ShaderCompiler.h"

using namespace Rorn::Engine;
using namespace Rorn::Maths;

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
        { "NORMAL",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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
	hr = ShaderCompiler::CompileShaderFromFile( L"Untextured.fx", "PS", "ps_4_0", &pPSBlob );
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

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(UntexturedSurfaceFormat::ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
    hr = device->CreateBuffer( &bd, NULL, &constantBuffer_ );
    if( FAILED( hr ) )
	{
		DiagnosticsManager::GetInstance().ReportError(hr, L"Error during creation of constant buffer");
        return hr;
	}

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
	const Matrix4x4& instanceToWorldMatrix, 
	const Matrix4x4& worldToProjectionMatrix,
	const Float4& ambientColor,
	const Float4& diffuseColor,
	const Float4& specularColor,
	float phongExponent)
{
    ConstantBuffer constantBuffer;
	constantBuffer.ModelToWorldMatrix = Matrix4x4::Transpose( instanceToWorldMatrix );
	constantBuffer.WorldToProjectionMatrix = Matrix4x4::Transpose( worldToProjectionMatrix );
	constantBuffer.AmbientColor = ambientColor;
	constantBuffer.DiffuseColor = diffuseColor;
	constantBuffer.SpecularColor = specularColor;
	constantBuffer.AmbientLightColor = LightingManager::GetInstance().GetAmbientLightColor();
	constantBuffer.MainLightDir = LightingManager::GetInstance().GetMainLight().Direction;
	constantBuffer.MainLightColor = LightingManager::GetInstance().GetMainLight().Color;
	constantBuffer.PhongExponent = phongExponent;
	constantBuffer.EyeDir = RenderManager::GetInstance().GetCurrentCameraEyeDir();
	deviceContext->UpdateSubresource( constantBuffer_, 0, NULL, &constantBuffer, 0, 0 );

    deviceContext->IASetInputLayout( vertexLayout_ );
	deviceContext->VSSetShader( vertexShader_, NULL, 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &constantBuffer_ );
	deviceContext->PSSetShader( pixelShader_, NULL, 0 );
	deviceContext->PSSetConstantBuffers( 0, 1, &constantBuffer_ );
}
