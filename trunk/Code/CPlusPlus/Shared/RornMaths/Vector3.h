#pragma once

namespace Rorn
{
	namespace Maths
	{
		class Vector3
		{
		public:
			// Data
			float X;
			float Y;
			float Z;

			// Construction
			Vector3(void);
			Vector3(float x, float y, float z);
			Vector3(const Vector3& source);

			// Assignment
			Vector3& operator=(const Vector3& source);

			// In-place binary operations
			Vector3& operator+=(const Vector3& rhs);
			Vector3& operator-=(const Vector3& rhs);
			Vector3& operator*=(float rhs);
			Vector3& operator/=(float rhs);

			// Member operations
			float GetLength() const;

			// Static member operations
			static Vector3 CrossProduct(const Vector3& lhs, const Vector3& rhs);// This is left-handed!
			static float DotProduct(const Vector3& lhs, const Vector3& rhs);
			static Vector3 GetUnitVector(const Vector3& source);
		};

		// Unary non-member operations
		static Vector3 operator-(const Vector3& source)
		{
			return Vector3(-source.X, -source.Y, -source.Z);
		}

		// Binary non-member operations
		static bool operator==(const Vector3& lhs, const Vector3& rhs)
		{
			return (lhs.X == rhs.X) && (lhs.Y == rhs.Y) && (lhs.Z == rhs.Z);
		}

		static bool operator!=(const Vector3& lhs, const Vector3& rhs)
		{
			return (lhs.X != rhs.X) || (lhs.Y != rhs.Y) || (lhs.Z != rhs.Z);
		}

		static Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
		{
			return Vector3(
				lhs.X + rhs.X,
				lhs.Y + rhs.Y,
				lhs.Z + rhs.Z);
		}

		static Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
		{
			return Vector3(
				lhs.X - rhs.X,
				lhs.Y - rhs.Y,
				lhs.Z - rhs.Z);
		}

		static Vector3 operator*(const Vector3& lhs, float rhs)
		{
			return Vector3(
				lhs.X * rhs,
				lhs.Y * rhs,
				lhs.Z * rhs);
		}

		static Vector3 operator*(float lhs, const Vector3& rhs)
		{
			return Vector3(
				lhs * rhs.X,
				lhs * rhs.Y,
				lhs * rhs.Z);
		}

		static Vector3 operator/(const Vector3& lhs, float rhs)
		{
			float rhsReciprocal = 1.0f / rhs;

			return Rorn::Maths::Vector3(
				lhs.X * rhsReciprocal,
				lhs.Y * rhsReciprocal,
				lhs.Z * rhsReciprocal);
		}

		static Vector3 operator/(float lhs, const Vector3& rhs)
		{
			float lhsReciprocal = 1.0f / lhs;

			return Rorn::Maths::Vector3(
				lhsReciprocal * rhs.X,
				lhsReciprocal * rhs.Y,
				lhsReciprocal * rhs.Z);
		}
	}
}

