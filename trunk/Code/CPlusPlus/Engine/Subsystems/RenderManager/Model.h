#pragma once

#include <list>

#include <d3d11.h>

#include "../../../Shared/RornMaths/Matrix4x4.h"

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
				const Maths::Matrix4x4& instanceToWorldMatrix, const Maths::Matrix4x4& worldToProjectionMatrix) const;

			void Release();
		private:
			enum SurfaceFormatType
			{
				Untextured = 0
			};

			Model(void);
			void LoadFromFile(const wchar_t* modelPathName, ID3D11Device* device);
			std::list<std::unique_ptr<RenderCommand>> renderCommands_;
		};
	}
}
