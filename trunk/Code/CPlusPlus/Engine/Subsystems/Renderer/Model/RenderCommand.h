#pragma once

#include "../../../../Shared/RornMaths/Float4.h"

#include "../Material/MaterialType.h"

namespace Rorn
{
	namespace Engine
	{
		class RenderCommand
		{
		public:
			RenderCommand(MaterialType::Type materialType, 
				unsigned int geometryType, 
				const Rorn::Maths::Float4& ambientColour, 
				const Rorn::Maths::Float4& diffuseColour, 
				const Rorn::Maths::Float4& specularColour, 
				float phongExponent, 
				unsigned int diffuseTextureId, 
				unsigned int vertexBufferId, 
				unsigned int indexBufferId,
				unsigned int vertexStride,
				unsigned int numIndices);
			~RenderCommand();

			MaterialType::Type GetMaterialType() const { return materialType_; }
			unsigned int GetGeometryType() const { return geometryType_; }
			const Rorn::Maths::Float4& GetAmbientColour() const { return ambientColour_; }
			const Rorn::Maths::Float4& GetDiffuseColour() const { return diffuseColour_; }
			const Rorn::Maths::Float4& GetSpecularColour() const { return specularColour_; }
			float GetPhongExponent() const { return phongExponent_; }
			unsigned int GetDiffuseTextureId() const { return diffuseTextureId_; }
			unsigned int GetVertexBufferId() const { return vertexBufferId_; }
			unsigned int GetIndexBufferId() const { return indexBufferId_; }
			unsigned int GetVertexStride() const { return vertexStride_; }
			unsigned int GetNumIndices() const { return numIndices_; }

			void SetNumIndices(unsigned int numIndices) { numIndices_ = numIndices; }
		private:
			RenderCommand(RenderCommand&);
			RenderCommand& operator=(RenderCommand&);

			MaterialType::Type materialType_;
			unsigned int geometryType_; 
			Rorn::Maths::Float4 ambientColour_;
			Rorn::Maths::Float4 diffuseColour_;
			Rorn::Maths::Float4 specularColour_;
			float phongExponent_;
			unsigned int diffuseTextureId_;
			unsigned int vertexBufferId_;
			unsigned int indexBufferId_;
			unsigned int vertexStride_;
			unsigned int numIndices_;
		};
	}
}