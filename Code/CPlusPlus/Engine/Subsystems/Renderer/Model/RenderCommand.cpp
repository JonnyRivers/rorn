#pragma once

#include "RenderCommand.h"

using namespace Rorn::Engine;

RenderCommand::RenderCommand(MaterialType::Type materialType, 
							 unsigned int geometryType, 
							 const Rorn::Maths::Float4& ambientColour, 
							 const Rorn::Maths::Float4& diffuseColour, 
							 const Rorn::Maths::Float4& specularColour, 
							 float phongExponent, 
							 unsigned int diffuseTextureId, 
							 unsigned int vertexBufferId, 
							 unsigned int indexBufferId,
							 unsigned int vertexStride,
							 unsigned int numIndices)
							 : materialType_(materialType),
							   geometryType_(geometryType),
							   ambientColour_(ambientColour),
							   diffuseColour_(diffuseColour),
							   specularColour_(specularColour),
							   phongExponent_(phongExponent),
							   diffuseTextureId_(diffuseTextureId),
							   vertexBufferId_(vertexBufferId),
							   indexBufferId_(indexBufferId),
							   vertexStride_(vertexStride),
							   numIndices_(numIndices)
{
}

RenderCommand::~RenderCommand()
{
}