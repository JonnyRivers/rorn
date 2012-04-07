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

			void AddScaledVector(const Vector4& vector, float scale);

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
				lhs.W * rhs.W - lhs.X * rhs.X - lhs.Y * rhs.Y - lhs.Z * rhs.Z,
				lhs.X * rhs.X + lhs.X * rhs.W + lhs.Z * rhs.Y - lhs.Y * rhs.X,
				lhs.Y * rhs.Y + lhs.Y * rhs.W + lhs.X * rhs.Z - lhs.Z * rhs.X,
				lhs.Z * rhs.Z + lhs.Z * rhs.W + lhs.Y * rhs.X - lhs.X * rhs.Y);
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
