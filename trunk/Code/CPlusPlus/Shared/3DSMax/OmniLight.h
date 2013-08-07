#pragma once

#include <Max.h>
#include <stdmat.h>

namespace Rorn
{
	namespace Max
	{
		class OmniLight
		{
		public:
			OmniLight(INode* node);

			Point3 GetColour() { return colour_; }
			float GetIntensity() { return intensity_; }
		private:
			Point3 colour_;
			float intensity_;
		};
	}
}