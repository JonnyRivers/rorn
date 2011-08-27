#pragma once

namespace Rorn
{
	namespace Maths
	{
		class Position
		{
		public:
			// Data
			float X;
			float Y;
			float Z;
			float W;

			// Construction
			Position(void);
			Position(float x, float y, float z);
			Position(const Position& source);

			// Assignment
			Position& operator=(const Position& source);

			// In-place binary operations
			Position& operator+=(const Position& rhs);
			Position& operator-=(const Position& rhs);
			Position& operator*=(float rhs);
			Position& operator/=(float rhs);

			// Member operations
			float GetLength() const;
		};

		// Unary non-member operations
		static Position operator-(const Position& source)
		{
			return Position(-source.X, -source.Y, -source.Z);
		}

		// Binary non-member operations
		static bool operator==(const Position& lhs, const Position& rhs)
		{
			return (lhs.X == rhs.X) && (lhs.Y == rhs.Y) && (lhs.Z == rhs.Z);
		}

		static bool operator!=(const Position& lhs, const Position& rhs)
		{
			return (lhs.X != rhs.X) || (lhs.Y != rhs.Y) || (lhs.Z != rhs.Z);
		}

		static Position operator+(const Position& lhs, const Position& rhs)
		{
			return Position(
				lhs.X + rhs.X,
				lhs.Y + rhs.Y,
				lhs.Z + rhs.Z);
		}

		static Position operator-(const Position& lhs, const Position& rhs)
		{
			return Position(
				lhs.X - rhs.X,
				lhs.Y - rhs.Y,
				lhs.Z - rhs.Z);
		}

		static Position operator*(const Position& lhs, float rhs)
		{
			return Position(
				lhs.X * rhs,
				lhs.Y * rhs,
				lhs.Z * rhs);
		}

		static Position operator*(float lhs, const Position& rhs)
		{
			return Position(
				lhs * rhs.X,
				lhs * rhs.Y,
				lhs * rhs.Z);
		}

		static Position operator/(const Position& lhs, float rhs)
		{
			float rhsReciprocal = 1.0f / rhs;

			return Position(
				lhs.X * rhsReciprocal,
				lhs.Y * rhsReciprocal,
				lhs.Z * rhsReciprocal);
		}

		static Position operator/(float lhs, const Position& rhs)
		{
			float lhsReciprocal = 1.0f / lhs;

			return Position(
				lhsReciprocal * rhs.X,
				lhsReciprocal * rhs.Y,
				lhsReciprocal * rhs.Z);
		}
	}
}

