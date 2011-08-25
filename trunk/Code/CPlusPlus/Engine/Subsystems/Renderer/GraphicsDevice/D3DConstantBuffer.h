#pragma once

#include <d3d11.h>

#include "IConstantBuffer.h"

namespace Rorn
{
	namespace Engine
	{
		// Simple RAII wrapper for ID3D11Buffer*
		class D3DConstantBuffer : public IConstantBuffer
		{
		public:
			D3DConstantBuffer(ID3D11Buffer* buffer);
			~D3DConstantBuffer();

			ID3D11Buffer* GetUnderlyingBuffer() { return buffer_; }
		private:
			D3DConstantBuffer(D3DConstantBuffer&);
			D3DConstantBuffer& operator=(D3DConstantBuffer&);

			ID3D11Buffer* buffer_;
		};
	}
}

