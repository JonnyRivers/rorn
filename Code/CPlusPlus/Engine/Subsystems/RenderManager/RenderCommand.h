#pragma once

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		class SurfaceFormat;

		class RenderCommand
		{
		public:
			enum PrimitiveType
			{
				TriangleList = 0
			};

			RenderCommand(const SurfaceFormat* surfaceFormat);
			~RenderCommand(void);

			void Draw(ID3D11DeviceContext* deviceContext, const XMMATRIX& instanceToWorldMatrix, const XMMATRIX& worldToProjectionMatrix) const;
		private:
			const SurfaceFormat* surfaceFormat_;
			D3D_PRIMITIVE_TOPOLOGY primitiveType_;
		};
	}
}
