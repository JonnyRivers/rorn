#pragma once

#include "Vector3.h"

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
			Vector3 GetMinimum() const;
			Vector3 GetMaximum() const;
			Vector3 GetCentre() const;
		private:
			Vector3 minimum_;
			Vector3 maximum_;
		};
	}
}
