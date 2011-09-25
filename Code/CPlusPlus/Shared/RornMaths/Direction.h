#pragma once

namespace Rorn
{
	namespace Maths
	{
		class UnitDirection;

		class Direction
		{
		public:
			// Data
			float X;
			float Y;
			float Z;
			float W;

			// Construction
			Direction(void);
			Direction(float x, float y, float z);
			Direction(const Direction& source);

			// Assignment
			Direction& operator=(const Direction& source);

			// In-place binary operations
			Direction& operator+=(const Direction& rhs);
			Direction& operator-=(const Direction& rhs);
			Direction& operator*=(float rhs);
			Direction& operator/=(float rhs);

			// Member operations
			float GetLength() const;

			// Static member operations
			static Direction CrossProduct(const Direction& lhs, const Direction& rhs);// This is left-handed!
			static float DotProduct(const Direction& lhs, const Direction& rhs);
			static UnitDirection Normalise(const Direction& source);
		};

		// Unary non-member operations
		static Direction operator-(const Direction& source)
		{
			return Direction(-source.X, -source.Y, -source.Z);
		}

		// Binary non-member operations
		static bool operator==(const Direction& lhs, const Direction& rhs)
		{
			return (lhs.X == rhs.X) && (lhs.Y == rhs.Y) && (lhs.Z == rhs.Z);
		}

		static bool operator!=(const Direction& lhs, const Direction& rhs)
		{
			return (lhs.X != rhs.X) || (lhs.Y != rhs.Y) || (lhs.Z != rhs.Z);
		}

		static Direction operator+(const Direction& lhs, const Direction& rhs)
		{
			return Direction(
				lhs.X + rhs.X,
				lhs.Y + rhs.Y,
				lhs.Z + rhs.Z);
		}

		static Direction operator-(const Direction& lhs, const Direction& rhs)
		{
			return Direction(
				lhs.X - rhs.X,
				lhs.Y - rhs.Y,
				lhs.Z - rhs.Z);
		}

		static Direction operator*(const Direction& lhs, float rhs)
		{
			return Direction(
				lhs.X * rhs,
				lhs.Y * rhs,
				lhs.Z * rhs);
		}

		static Direction operator*(float lhs, const Direction& rhs)
		{
			return Direction(
				lhs * rhs.X,
				lhs * rhs.Y,
				lhs * rhs.Z);
		}

		static Direction operator/(const Direction& lhs, float rhs)
		{
			float rhsReciprocal = 1.0f / rhs;

			return Direction(
				lhs.X * rhsReciprocal,
				lhs.Y * rhsReciprocal,
				lhs.Z * rhsReciprocal);
		}

		static Direction operator/(float lhs, const Direction& rhs)
		{
			float lhsReciprocal = 1.0f / lhs;

			return Direction(
				lhsReciprocal * rhs.X,
				lhsReciprocal * rhs.Y,
				lhsReciprocal * rhs.Z);
		}
	}
}

