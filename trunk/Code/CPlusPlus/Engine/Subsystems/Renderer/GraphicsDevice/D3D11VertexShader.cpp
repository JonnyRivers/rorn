#pragma once

#include "D3D11VertexShader.h"

#include <vector>

#include <d3dcompiler.h>
#include <D3DX11async.h>

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../../Exceptions/initialisation_exception.h"

#include "../Material/VertexFormatFlags.h"

using namespace Rorn::Engine;

D3D11VertexShader::D3D11VertexShader(IDiagnostics* diagnostics, ID3D11Device* device, const wchar_t* pathname, unsigned int vertexFormatFlags)
{
	ID3DBlob* pVSBlob = CompileShaderFromFile( diagnostics, pathname, "VS", "vs_4_0" );
	
	HRESULT hr = device->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &VertexShader );
    if( FAILED( hr ) )
	{
		pVSBlob->Release();
		diagnostics->GetLoggingStream() << "Unable to create vertex shader from data blob" << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
        throw initialisation_exception("Unable to create vertex shader from data blob.");
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;
	int vertexPositionStart = 0;
	if((vertexFormatFlags & VertexFormatFlags::Position) == VertexFormatFlags::Position )
	{
		D3D11_INPUT_ELEMENT_DESC positionInputDesc = { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, vertexPositionStart, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		vertexPositionStart += 16;
		inputElements.push_back(positionInputDesc);
	}
	if((vertexFormatFlags & VertexFormatFlags::Normal) == VertexFormatFlags::Normal )
	{
		D3D11_INPUT_ELEMENT_DESC normalInputDesc = { "NORMAL",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, vertexPositionStart, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		vertexPositionStart += 16;
		inputElements.push_back(normalInputDesc);
	}
	if((vertexFormatFlags & VertexFormatFlags::UV0) == VertexFormatFlags::UV0 )
	{
		D3D11_INPUT_ELEMENT_DESC uv0InputDesc = { "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, vertexPositionStart, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		vertexPositionStart += 8;
		inputElements.push_back(uv0InputDesc);
	}

	hr = device->CreateInputLayout( &inputElements[0], static_cast<UINT>(inputElements.size()), pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &VertexLayout );
	if( FAILED( hr ) )
	{
		pVSBlob->Release();
		diagnostics->GetLoggingStream() << "Unable to create vertex input layout from data blob" << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
        throw initialisation_exception("Unable to create vertex input layout from data blob.");
	}

	pVSBlob->Release();
}

D3D11VertexShader::~D3D11VertexShader()
{
	VertexLayout->Release();
	VertexShader->Release();
}

ID3DBlob* D3D11VertexShader::CompileShaderFromFile(IDiagnostics* diagnostics, const wchar_t* fileName, const char* entryPoint, const char* shaderModel)
{
	ID3DBlob* result;

    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif
    ID3DBlob* pErrorBlob;
    HRESULT hr = ::D3DX11CompileFromFile( fileName, NULL, NULL, entryPoint, shaderModel, 
        dwShaderFlags, 0, NULL, &result, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
		{
			diagnostics->GetLoggingStream() << "Error during compilation of shader '" << entryPoint << "' in file '" << fileName << 
				"' using shader model '" << shaderModel << "'." << std::endl;
			diagnostics->GetLoggingStream() << "Reason: " << static_cast<const char*>(pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}

		throw initialisation_exception("Unable to compile shader.");
    }

    if( pErrorBlob != NULL )
	{
		pErrorBlob->Release();
	}

	diagnostics->GetLoggingStream() << "Successfully compiled shader '" << entryPoint << "' in file '" << fileName << "' using shader model '" << 
		shaderModel << "'." << std::endl;

    return result;
}