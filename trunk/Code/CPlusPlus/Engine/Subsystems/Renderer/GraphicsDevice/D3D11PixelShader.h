#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11PixelShader
		{
		public:
			D3D11PixelShader(IDiagnostics* diagnostics, ID3D11Device* device, const wchar_t* pathname);
			~D3D11PixelShader();

			ID3D11PixelShader* PixelShader;
		private:
			// The tools should do this, not the engine!  It's also duplicated, but it will be gone soon enough...
			ID3DBlob* CompileShaderFromFile(IDiagnostics* diagnostics, const wchar_t* fileName, const char* entryPoint, const char* shaderModel);

			D3D11PixelShader(D3D11PixelShader&);
			D3D11PixelShader& operator=(D3D11PixelShader&);
		};
	}
}

