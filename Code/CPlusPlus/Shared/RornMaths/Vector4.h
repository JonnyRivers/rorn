#pragma once

namespace Rorn
{
	namespace Maths
	{
		class Vector4
		{
		public:
			// Data
			float X;
			float Y;
			float Z;
			float W;

			// Construction
			Vector4(void);
			Vector4(float x, float y, float z, float w);
			Vector4(const Vector4& source);

			// Assignment
			Vector4& operator=(const Vector4& source);

			// In-place binary operations
			Vector4& operator+=(const Vector4& rhs);
			Vector4& operator-=(const Vector4& rhs);
			Vector4& operator*=(float rhs);
			Vector4& operator/=(float rhs);

			// Member operations
			float GetLength() const;

			// Static member operations
			static Vector4 CrossProduct(const Vector4& lhs, const Vector4& rhs);
			static float DotProduct(const Vector4& lhs, const Vector4& rhs);
			static Vector4 Normalise(const Vector4& source);
		};

		// Unary non-member operations
		static Vector4 operator-(const Vector4& source)
		{
			return Vector4(-source.X, -source.Y, -source.Z, source.W);// Negating W makes no sense
		}

		// Binary non-member operations
		static bool operator==(const Vector4& lhs, const Vector4& rhs)
		{
			return (lhs.X == rhs.X) && (lhs.Y == rhs.Y) && (lhs.Z == rhs.Z) && (lhs.Z == rhs.Z);
		}

		static bool operator!=(const Vector4& lhs, const Vector4& rhs)
		{
			return (lhs.X != rhs.X) || (lhs.Y != rhs.Y) || (lhs.Z != rhs.Z) || (lhs.Z != rhs.Z);
		}

		static Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
		{
			return Vector4(
				lhs.X + rhs.X,
				lhs.Y + rhs.Y,
				lhs.Z + rhs.Z,
				lhs.W + rhs.W);
		}

		static Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
		{
			return Vector4(
				lhs.X - rhs.X,
				lhs.Y - rhs.Y,
				lhs.Z - rhs.Z,
				lhs.W - rhs.W);
		}

		static Vector4 operator*(const Vector4& lhs, float rhs)
		{
			return Vector4(
				lhs.X * rhs,
				lhs.Y * rhs,
				lhs.Z * rhs,
				lhs.W * rhs);
		}

		static Vector4 operator*(float lhs, const Vector4& rhs)
		{
			return Vector4(
				lhs * rhs.X,
				lhs * rhs.Y,
				lhs * rhs.Z,
				lhs * rhs.W);
		}

		static Vector4 operator/(const Vector4& lhs, float rhs)
		{
			float rhsReciprocal = 1.0f / rhs;

			return Vector4(
				lhs.X * rhsReciprocal,
				lhs.Y * rhsReciprocal,
				lhs.Z * rhsReciprocal,
				lhs.W * rhsReciprocal);
		}

		static Vector4 operator/(float lhs, const Vector4& rhs)
		{
			float lhsReciprocal = 1.0f / lhs;

			return Vector4(
				lhsReciprocal * rhs.X,
				lhsReciprocal * rhs.Y,
				lhsReciprocal * rhs.Z,
				lhsReciprocal * rhs.W);
		}
	}
}

