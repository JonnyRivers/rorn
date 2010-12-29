#pragma once

#include <list>

#include <windows.h>

#include <d3d11.h>
#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		class RenderCommand;

		class Model
		{
		public:
			Model(void);
			~Model(void);

			void Draw(ID3D11DeviceContext* deviceContext, 
				const XMMATRIX& instanceToWorldMatrix, const XMMATRIX& worldToProjectionMatrix) const;
		private:
			std::list<RenderCommand> renderCommands_;
		};
	}
}
