#pragma once

#include <map>

#include <d3d11.h>

#include <Windows.h>

#include "../../../Exceptions/initialisation_exception.h"

#include "../../../Interfaces/IDiagnostics.h"

#include "IGraphicsDevice.h"

#include "D3D11BlendState.h"
#include "D3D11Buffer.h"
#include "D3D11DepthStencilView.h"
#include "D3D11Device.h"
#include "D3D11PixelShader.h"
#include "D3D11RenderTargetView.h"
#include "D3D11SamplerState.h"
#include "D3D11ShaderResourceView.h"
#include "D3D11Texture2D.h"
#include "D3D11VertexShader.h"
#include "D3D11Viewport.h"
#include "OutputDimensions.h"

namespace Rorn
{
	namespace Engine
	{
		class D3DGraphicsDevice : public IGraphicsDevice
		{
		public:
			D3DGraphicsDevice(HWND applicationWindowHandle, IDiagnostics* diagnostics);
			~D3DGraphicsDevice();

			virtual unsigned int CreateConstantBuffer(unsigned int dataSize);
			virtual unsigned int CreateIndexBuffer(const void* data, unsigned int dataSize);
			virtual unsigned int CreatePixelShader(const wchar_t* shaderFilename);
			virtual unsigned int CreateSamplerState();
			virtual unsigned int CreateTexture(const void* data, unsigned int dataSize);
			virtual unsigned int CreateVertexBuffer(const void* data, unsigned int dataSize);
			virtual unsigned int CreateVertexShader(const wchar_t* shaderFilename, unsigned int vertexFormatFlags);

			virtual void UpdateConstantBufferData(unsigned int constantBufferId, const void* data);
			virtual void UpdateVertexBufferData(unsigned int vertexBufferId, const void* data);
			virtual void UpdateIndexBufferData(unsigned int indexBufferId, const void* data);

			virtual void SetPixelShader(unsigned int pixelShaderId);
			virtual void SetPixelShaderConstantBuffer(unsigned int constantBufferId);
			virtual void SetSamplerState(unsigned int samplerStateId);
			virtual void SetVertexShader(unsigned int vertexShaderId);
			virtual void SetVertexShaderConstantBuffer(unsigned int constantBufferId);
			virtual void SetVertexBuffer(unsigned int vertexBufferId, unsigned int vertexStride);
			virtual void SetIndexBuffer(unsigned int indexBufferId);
			virtual void SetTexture(unsigned int textureId);

			virtual unsigned int GetOutputWidth() const;
			virtual unsigned int GetOutputHeight() const;
			virtual float GetOutputAspectRatio() const;

			virtual void ClearView(const Rorn::Maths::Float4& backColour);
			virtual void DrawIndexed(unsigned int numIndices);
			virtual void Present();
		private:
			D3DGraphicsDevice(D3DGraphicsDevice&);
			D3DGraphicsDevice& operator=(D3DGraphicsDevice&);

			IDiagnostics* diagnostics_;

			// Core, 'fixed' data
			OutputDimensions outputDimensions_;
			D3D11Device device_;
			D3D11RenderTargetView renderTargetView_;
			D3D11Texture2D depthStencilTexture_;
			D3D11DepthStencilView depthStencilView_;
			D3D11BlendState blendState_;
			D3D11Viewport viewport_;

			// Data created at the request of clients
			std::map<unsigned int, std::unique_ptr<D3D11Buffer>> constantBuffers_;
			std::map<unsigned int, std::unique_ptr<D3D11Buffer>> indexBuffers_;
			std::map<unsigned int, std::unique_ptr<D3D11PixelShader>> pixelShaders_;
			std::map<unsigned int, std::unique_ptr<D3D11SamplerState>> samplerStates_;
			std::map<unsigned int, std::unique_ptr<D3D11ShaderResourceView>> textures_;
			std::map<unsigned int, std::unique_ptr<D3D11Buffer>> vertexBuffers_;
			std::map<unsigned int, std::unique_ptr<D3D11VertexShader>> vertexShaders_;
		};
	}
}
