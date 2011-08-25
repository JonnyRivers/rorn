#pragma once

#include "../../../../Shared/RornMaths/Float4.h"

namespace Rorn
{
	namespace Engine
	{
		struct IGraphicsDevice
		{
		public:
			virtual ~IGraphicsDevice() {}

			virtual unsigned int CreateConstantBuffer(unsigned int dataSize) = 0;
			virtual unsigned int CreateIndexBuffer(const void* data, unsigned int dataSize) = 0;
			virtual unsigned int CreatePixelShader(const wchar_t* shaderFilename) = 0;
			virtual unsigned int CreateSamplerState() = 0;
			virtual unsigned int CreateTexture(const void* data, unsigned int dataSize) = 0;
			virtual unsigned int CreateVertexShader(const wchar_t* shaderFilename, unsigned int vertexFormatFlags) = 0;
			virtual unsigned int CreateVertexBuffer(const void* data, unsigned int dataSize) = 0;

			virtual void UpdateConstantBufferData(unsigned int constantBufferId, const void* data) = 0;
			virtual void UpdateVertexBufferData(unsigned int vertexBufferId, const void* data) = 0;
			virtual void UpdateIndexBufferData(unsigned int indexBufferId, const void* data) = 0;

			virtual void SetPixelShader(unsigned int pixelShaderId) = 0;
			virtual void SetPixelShaderConstantBuffer(unsigned int constantBufferId) = 0;
			virtual void SetSamplerState(unsigned int samplerStateId) = 0;
			virtual void SetVertexShader(unsigned int vertexShaderId) = 0;
			virtual void SetVertexShaderConstantBuffer(unsigned int constantBufferId) = 0;
			virtual void SetVertexBuffer(unsigned int vertexBufferId, unsigned int vertexStride) = 0;
			virtual void SetIndexBuffer(unsigned int indexBufferId) = 0;
			virtual void SetTexture(unsigned int textureId) = 0;

			virtual unsigned int GetOutputWidth() const = 0;
			virtual unsigned int GetOutputHeight() const = 0;
			virtual float GetOutputAspectRatio() const = 0;

			virtual void ClearView(const Rorn::Maths::Float4& backColour) = 0;
			virtual void DrawIndexed(unsigned int numIndices) = 0;
			virtual void Present() = 0;
		};
	}
}
