#pragma once

#include "Position.h"

namespace Rorn
{
	namespace Maths
	{
		class BoundingBox
		{
		public:
			BoundingBox();
			BoundingBox(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

			float GetBoundingRadius() const;
			Position GetMinimum() const;
			Position GetMaximum() const;
			Position GetCentre() const;
		private:
			Position minimum_;
			Position maximum_;
		};
	}
}
