#pragma once

#include <Max.h>
#include <stdmat.h>

namespace Rorn
{
	namespace Max
	{
		class BoxPrimitive
		{
		public:
			BoxPrimitive(INode* node);

			float GetLength() { return length_; }
			float GetWidth() { return width_; }
			float GetHeight() { return height_; }
		private:
			float length_;
			float width_;
			float height_;
		};
	}
}