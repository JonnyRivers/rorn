#pragma once

#include <d3d11.h>

#include "IVertexShader.h"

namespace Rorn
{
	namespace Engine
	{
		// Simple RAII wrapper for ID3D11VertexShader* and ID3D11InputLayout*
		class D3DVertexShader : public IVertexShader
		{
		public:
			D3DVertexShader(ID3D11VertexShader* vertexShader, ID3D11InputLayout* vertexInput);
			~D3DVertexShader();

			ID3D11VertexShader* GetUnderlyingShader() { return vertexShader_; }
			ID3D11InputLayout* GetUnderlyingInputLayout() { return vertexInput_; }
		private:
			D3DVertexShader(D3DVertexShader&);
			D3DVertexShader& operator=(D3DVertexShader&);

			ID3D11VertexShader* vertexShader_;
			ID3D11InputLayout* vertexInput_;
		};
	}
}

