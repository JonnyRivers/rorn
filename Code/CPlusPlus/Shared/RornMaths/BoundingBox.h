#pragma once

#include "Vector4.h"

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
			Vector4 GetMinimum() const;
			Vector4 GetMaximum() const;
			Vector4 GetCentre() const;
		private:
			Vector4 minimum_;
			Vector4 maximum_;
		};
	}
}
