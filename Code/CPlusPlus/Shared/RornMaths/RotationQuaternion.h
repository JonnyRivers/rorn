#pragma once

namespace Rorn
{
	namespace Maths
	{
		class RotationQuaternion
		{
		public:
			// Data
			// W is assumed to be 0 for rotation quaternions
			float X;
			float Y;
			float Z;

			RotationQuaternion(float x, float y, float z);
		};

		static RotationQuaternion operator-(const RotationQuaternion& source)
		{
			return RotationQuaternion(-source.X, -source.Y, -source.Z);
		}

		static RotationQuaternion operator*(const RotationQuaternion& lhs, float rhs)
		{
			return RotationQuaternion(
				lhs.X * rhs,
				lhs.Y * rhs,
				lhs.Z * rhs);
		}

		static RotationQuaternion operator*(float lhs, const RotationQuaternion& rhs)
		{
			return RotationQuaternion(
				lhs * rhs.X,
				lhs * rhs.Y,
				lhs * rhs.Z);
		}
	}
}
