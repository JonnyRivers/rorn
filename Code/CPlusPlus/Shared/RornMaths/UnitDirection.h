#pragma once

namespace Rorn
{
	namespace Maths
	{
		class UnitDirection
		{
		public:
			// Data
			float X;
			float Y;
			float Z;
			float W;

			// Construction
			UnitDirection(void);
			UnitDirection(float x, float y, float z);
			UnitDirection(const UnitDirection& source);

			// Assignment
			UnitDirection& operator=(const UnitDirection& source);

			// Static member operations
			static UnitDirection CrossProduct(const UnitDirection& lhs, const UnitDirection& rhs);// This is left-handed!
			static float DotProduct(const UnitDirection& lhs, const UnitDirection& rhs);
		};

		// Binary non-member operations
		static bool operator==(const UnitDirection& lhs, const UnitDirection& rhs)
		{
			return (lhs.X == rhs.X) && (lhs.Y == rhs.Y) && (lhs.Z == rhs.Z);
		}

		static bool operator!=(const UnitDirection& lhs, const UnitDirection& rhs)
		{
			return (lhs.X != rhs.X) || (lhs.Y != rhs.Y) || (lhs.Z != rhs.Z);
		}
	}
}

