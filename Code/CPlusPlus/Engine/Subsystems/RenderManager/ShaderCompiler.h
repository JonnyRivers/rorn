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
			static HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
		};
	}
}
