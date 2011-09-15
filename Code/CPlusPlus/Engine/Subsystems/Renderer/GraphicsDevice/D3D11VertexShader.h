#pragma once

#include <d3d11.h>

#include "../../../Interfaces/IDiagnostics.h"

namespace Rorn
{
	namespace Engine
	{
		class D3D11VertexShader
		{
		public:
			D3D11VertexShader(IDiagnostics* diagnostics, ID3D11Device* device, const wchar_t* pathname, unsigned int vertexFormatFlags);
			~D3D11VertexShader();

			ID3D11VertexShader* VertexShader;
			ID3D11InputLayout* VertexLayout;
		private:
			// The tools should do this, not the engine!  It's also duplicated, but it will be gone soon enough...
			ID3DBlob* CompileShaderFromFile(IDiagnostics* diagnostics, const wchar_t* fileName, const char* entryPoint, const char* shaderModel);

			D3D11VertexShader(D3D11VertexShader&);
			D3D11VertexShader& operator=(D3D11VertexShader&);
		};
	}
}

