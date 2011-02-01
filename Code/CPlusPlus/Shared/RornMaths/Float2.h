#pragma once

namespace Rorn
{
	namespace Maths
	{
		class Float2
		{
		public:
			// Data
			float X;
			float Y;

			// Construction
			Float2(void);
			Float2(float x, float y);
			Float2(const Float2& source);

			// Assignment
			Float2& operator=(const Float2& source);
		};
	}
}

