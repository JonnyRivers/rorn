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
			friend class RenderManager;

		public:
			~Model(void);

			void Draw(ID3D11DeviceContext* deviceContext, 
				const XMMATRIX& instanceToWorldMatrix, const XMMATRIX& worldToProjectionMatrix) const;
		private:
			Model(void);
			void LoadFromFile(const char* modelPathName);
			std::list<std::unique_ptr<RenderCommand>> renderCommands_;
		};
	}
}
