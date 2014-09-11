#pragma once

#include "D3D11PixelShader.h"

#include <d3dcompiler.h>
#include <D3DX11async.h>

#include "../../../../Shared/ErrorCodes/HResultFormatter.h"

#include "../../../Exceptions/initialisation_exception.h"

using namespace Rorn::Engine;

D3D11PixelShader::D3D11PixelShader(IDiagnostics* diagnostics, ID3D11Device* device, const wchar_t* pathname)
{
	ID3DBlob* pPSBlob = CompileShaderFromFile( diagnostics, pathname, "PS", "ps_4_0" );

	HRESULT hr = device->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &PixelShader );
	pPSBlob->Release();
    if( FAILED( hr ) )
	{
		diagnostics->GetLoggingStream() << "Unable to create pixel shader from data blob.  HRESULT details follow." << std::endl;
		diagnostics->GetLoggingStream() << Rorn::ErrorCodes::HResultFormatter::FormatHResult(hr);
        throw initialisation_exception("Unable to create pixel shader from data blob.");
	}
}

D3D11PixelShader::~D3D11PixelShader()
{
	PixelShader->Release();
}

ID3DBlob* D3D11PixelShader::CompileShaderFromFile(IDiagnostics* diagnostics, const wchar_t* fileName, const char* entryPoint, const char* shaderModel)
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