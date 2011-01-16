#pragma once

#include "RenderCommand.h"

#include <windows.h>

#include <d3d11.h>

#include "../../../Shared/RornMaths/Float4.h"
#include "../../../Shared/RornMaths/Matrix4x4.h"
#include "../../../Shared/RornMaths/Vector3.h"

#include "../FileManager/FileReader.h"

namespace Rorn
{
	namespace Engine
	{
		class UntexturedRenderCommand : public RenderCommand
		{
		public:
			UntexturedRenderCommand(void);
			~UntexturedRenderCommand(void);

			HRESULT LoadFromFile(FileReader& fileReader, ID3D11Device* device);

			virtual void Draw(ID3D11DeviceContext* deviceContext, 
				const Maths::Matrix4x4& instanceToWorldMatrix, const Maths::Matrix4x4& worldToProjectionMatrix);

			virtual void Release();
		private:
			struct VertexFormat
			{
				Maths::Vector3 Position;
				Maths::Vector3 Normal;
			};

			Maths::Float4 ambientColor_;
			Maths::Float4 diffuseColor_;
			Maths::Float4 specularColor_;
			float phongExponent_;

			int vertexCount_;
			int vertexDataSize_;
			ID3D11Buffer* vertexBuffer_;
			int indexCount_;
			int indexDataSize_;
			ID3D11Buffer* indexBuffer_;
		};
	}
}

