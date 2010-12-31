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

			void Release();
		private:
			enum SurfaceFormatType
			{
				Untextured = 0
			};

			Model(void);
			void LoadFromFile(const char* modelPathName, ID3D11Device* device);
			std::list<std::unique_ptr<RenderCommand>> renderCommands_;
		};
	}
}
