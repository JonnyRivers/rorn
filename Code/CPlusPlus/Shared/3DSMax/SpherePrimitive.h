#pragma once

#include <Max.h>
#include <stdmat.h>

namespace Rorn
{
	namespace Max
	{
		class SpherePrimitive
		{
		public:
			SpherePrimitive(INode* node);

			float GetRadius() { return radius_; }
		private:
			float radius_;
		};
	}
}