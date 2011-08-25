#pragma once

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../Interfaces/IDiagnostics.h"

#include "../GraphicsDevice/IGraphicsDevice.h"

namespace Rorn
{
	namespace Engine
	{
		class Material
		{
		public:
			Material(IDiagnostics* diagnostics, IGraphicsDevice* graphicsDevice, const wchar_t* shaderFilename, unsigned int vertexFormatFlags, unsigned int constantBufferSize);
			~Material();

			unsigned int GetVertexShaderId() const { return vertexShaderId_; }
			unsigned int GetPixelShaderId() const { return pixelShaderId_; }
			bool HasConstantBuffer() const { return hasConstantBuffer_; }
			unsigned int GetConstantBufferId() const { return constantBufferId_; }
			bool HasDiffuseMap() const { return hasDiffuseMap_; }
			unsigned int GetDiffuseSamplerStateId() const { return diffuseSamplerStateId_; }
		private:
			Material(Material&);
			Material& operator=(Material&);

			IDiagnostics* diagnostics_;
			unsigned int vertexShaderId_;
			unsigned int pixelShaderId_;
			bool hasConstantBuffer_;
			unsigned int constantBufferId_;
			bool hasDiffuseMap_;
			unsigned int diffuseSamplerStateId_;
		};
	}
}