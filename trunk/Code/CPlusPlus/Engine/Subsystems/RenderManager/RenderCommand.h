#pragma once

#include <d3d11.h>

#include "../../../Shared/RornMaths/Matrix4x4.h"

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
				const Maths::Matrix4x4& instanceToWorldMatrix, const Maths::Matrix4x4& worldToProjectionMatrix) = 0;

			virtual void Release() = 0;
		private:
		};
	}
}
