#pragma once

#include <windows.h>
#include <d3d11.h>

namespace Rorn
{
	namespace Engine
	{
		class ShaderCompiler
		{
		public:
			static HRESULT CompileShaderFromFile(const wchar_t* fileName, const char* entryPoint, 
				const char* shaderModel, ID3DBlob** blob);
		};
	}
}
