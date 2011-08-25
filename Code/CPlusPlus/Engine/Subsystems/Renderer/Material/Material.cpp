#pragma once

#include "../../../Exceptions/initialisation_exception.h"

#include "Material.h"
#include "VertexFormatFlags.h"

using namespace Rorn::Engine;

Material::Material(IDiagnostics* diagnostics, IGraphicsDevice* graphicsDevice, const wchar_t* shaderFilename, unsigned int vertexFormatFlags, unsigned int constantBufferSize)  : diagnostics_(diagnostics)
{
	diagnostics_->GetLoggingStream() << "Material instance is being created." << std::endl;

	vertexShaderId_ = graphicsDevice->CreateVertexShader(shaderFilename, vertexFormatFlags);
	pixelShaderId_ = graphicsDevice->CreatePixelShader(shaderFilename);
	hasConstantBuffer_ = (constantBufferSize > 0);
	if(hasConstantBuffer_)
	{
		constantBufferId_ = graphicsDevice->CreateConstantBuffer(constantBufferSize);
	}
	hasDiffuseMap_ = ((vertexFormatFlags & VertexFormatFlags::UV0) == VertexFormatFlags::UV0);
	if(hasDiffuseMap_)
	{
		diffuseSamplerStateId_ = graphicsDevice->CreateSamplerState();
	}

	diagnostics_->GetLoggingStream() << "Material instance was created successfully." << std::endl;
}

Material::~Material()
{
	diagnostics_->GetLoggingStream() << "Material instance is being destroyed." << std::endl;
}
