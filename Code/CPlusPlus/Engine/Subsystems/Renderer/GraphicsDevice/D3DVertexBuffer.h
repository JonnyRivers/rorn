#pragma once

#include <d3d11.h>

#include "IVertexBuffer.h"

namespace Rorn
{
	namespace Engine
	{
		// Simple RAII wrapper for ID3D11Buffer*
		class D3DVertexBuffer : public IVertexBuffer
		{
		public:
			D3DVertexBuffer(ID3D11Buffer* buffer);
			~D3DVertexBuffer();

			ID3D11Buffer* GetUnderlyingBuffer() { return buffer_; }
		private:
			D3DVertexBuffer(D3DVertexBuffer&);
			D3DVertexBuffer& operator=(D3DVertexBuffer&);

			ID3D11Buffer* buffer_;
		};
	}
}

