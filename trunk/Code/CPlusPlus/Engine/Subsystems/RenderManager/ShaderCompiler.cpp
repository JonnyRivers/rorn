#include "ShaderCompiler.h"

#include "../DiagnosticsManager/DiagnosticsManager.h"

#include <sstream>

#include <d3dcompiler.h>
#include <d3dx11async.h>

using namespace Rorn::Engine;

/*static*/ HRESULT ShaderCompiler::CompileShaderFromFile(const wchar_t* fileName, const char* entryPoint, const char* shaderModel, ID3DBlob** blob)
{
	HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
    ID3DBlob* pErrorBlob;
    hr = D3DX11CompileFromFile( fileName, NULL, NULL, entryPoint, shaderModel, 
        dwShaderFlags, 0, NULL, blob, &pErrorBlob, NULL );
    if( FAILED(hr) )
    {
        if( pErrorBlob != NULL )
		{
			std::wstringstream errorStream;
			errorStream << "Error during compilation of shader '" << entryPoint << "' in file '" << fileName << 
				"' using shader model '" << shaderModel << "'." << std::endl;
			errorStream << "Reason: " << static_cast<const char*>(pErrorBlob->GetBufferPointer());

			DiagnosticsManager::GetInstance().ReportError(errorStream.str().c_str());

			pErrorBlob->Release();
		}

        return hr;
    }

    if( pErrorBlob ) 
		pErrorBlob->Release();

	DiagnosticsManager::GetInstance().GetLoggingStream() << "Successfully compiled shader '" << entryPoint << 
		"' in file '" << fileName << "' using shader model '" << shaderModel << "'." << std::endl;

    return S_OK;
}