#pragma once

#include <Max.h>
#include <stdmat.h>

namespace Rorn
{
	namespace Max
	{
		class CylinderPrimitive
		{
		public:
			CylinderPrimitive(INode* node);

			float GetRadius() { return radius_; }
			float GetHeight() { return height_; }
		private:
			float radius_;
			float height_;
		};
	}
}