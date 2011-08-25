#pragma once

#include <d3d11.h>

#include "IIndexBuffer.h"

namespace Rorn
{
	namespace Engine
	{
		// Simple RAII wrapper for ID3D11Buffer*
		class D3DIndexBuffer : public IIndexBuffer
		{
		public:
			D3DIndexBuffer(ID3D11Buffer* buffer);
			~D3DIndexBuffer();

			ID3D11Buffer* GetUnderlyingBuffer() { return buffer_; }
		private:
			D3DIndexBuffer(D3DIndexBuffer&);
			D3DIndexBuffer& operator=(D3DIndexBuffer&);

			ID3D11Buffer* buffer_;
		};
	}
}

