#pragma once

#include <vector>

#include "../../../../Shared/RornMaths/BoundingBox.h"

#include "../../../Interfaces/IModel.h"

namespace Rorn
{
	namespace Engine
	{
		class Model : public IModel
		{
		public:
			Model(const Maths::BoundingBox& boundingBox, const std::vector<unsigned int>& texturesCreated, const std::vector<unsigned int>& renderCommands, unsigned int boundsId);
			~Model();

			virtual const Maths::BoundingBox& GetBoundingBox() const;
			virtual unsigned int GetBoundsId() const;

			const std::vector<unsigned int>& GetRenderCommandIds() const { return renderCommands_; }
		private:
			Model(Model&);
			Model& operator=(Model&);

			Rorn::Maths::BoundingBox boundingBox_;
			std::vector<unsigned int> texturesCreated_;
			std::vector<unsigned int> renderCommands_;
			unsigned int boundsId_;
		};
	}
}