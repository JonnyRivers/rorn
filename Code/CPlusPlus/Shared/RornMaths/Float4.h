#pragma once

namespace Rorn
{
	namespace Maths
	{
		class Float4
		{
		public:
			// Data
			float X;
			float Y;
			float Z;
			float W;

			// Construction
			Float4(void);
			Float4(float x, float y, float z, float w);
			Float4(const Float4& source);

			// Assignment
			Float4& operator=(const Float4& source);
		};
	}
}

