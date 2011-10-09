#pragma once

namespace Rorn
{
	namespace Maths
	{
		class Vector4;

		class Quaternion
		{
		public:
			// Data
			float W;
			float X;
			float Y;
			float Z;

			Quaternion();
			Quaternion(float w, float x, float y, float z);

			float GetLength() const;

			void ToAxisAngle(Vector4& axis, float& angle);

			static Quaternion CreateFromAxisAngle(const Vector4& axis, float angle);
			static Quaternion Normalise(const Quaternion& source);
		};

		static Quaternion operator+(const Quaternion& lhs, const Quaternion& rhs)
		{
			return Quaternion(
				lhs.W + rhs.W,
				lhs.X + rhs.X,
				lhs.Y + rhs.Y,
				lhs.Z + rhs.Z);
		}

		static Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
		{
			return Quaternion(
				- lhs.X * rhs.X - lhs.Y * rhs.Y - lhs.Z * rhs.Z,
				  lhs.X * rhs.W + lhs.Y * rhs.Z - lhs.Z * rhs.Y,
				- lhs.X * rhs.Z + lhs.Y * rhs.W + lhs.Z * rhs.X,
				  lhs.X * rhs.Y - lhs.Y * rhs.X - lhs.Z * rhs.W);// Is this an orientation?
		}

		static Quaternion operator*(const Quaternion& lhs, float rhs)
		{
			return Quaternion(
				lhs.W * rhs,
				lhs.X * rhs,
				lhs.Y * rhs,
				lhs.Z * rhs);
		}

		static Quaternion operator*(float lhs, const Quaternion& rhs)
		{
			return Quaternion(
				lhs * rhs.W,
				lhs * rhs.X,
				lhs * rhs.Y,
				lhs * rhs.Z);
		}			
	}
}
