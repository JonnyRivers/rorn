#pragma once

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		class RenderCommand
		{
		public:
			enum PrimitiveType
			{
				TriangleList = 0
			};

			RenderCommand(void);
			~RenderCommand(void);

			virtual void Draw(ID3D11DeviceContext* deviceContext, 
				CXMMATRIX instanceToWorldMatrix, CXMMATRIX worldToProjectionMatrix) = 0;

			virtual void Release() = 0;
		private:
		};
	}
}
