#pragma once

#include "RotationQuaternion.h"

namespace Rorn
{
	namespace Maths
	{
		class UnitDirection;

		class OrientationQuaternion
		{
		public:
			// Data
			float W;
			float X;
			float Y;
			float Z;

			OrientationQuaternion(float w, float x, float y, float z);
			OrientationQuaternion(const UnitDirection& axis, float theta);
		};

		static OrientationQuaternion operator+(const OrientationQuaternion& lhs, const OrientationQuaternion& rhs)
		{
			return OrientationQuaternion(
				lhs.W + rhs.W,
				lhs.X + rhs.X,
				lhs.Y + rhs.Y,
				lhs.Z + rhs.Z);
		}

		static OrientationQuaternion operator*(const RotationQuaternion& lhs, const OrientationQuaternion& rhs)
		{
			return OrientationQuaternion(
				- lhs.X * rhs.X - lhs.Y * rhs.Y - lhs.Z * rhs.Z,
				  lhs.X * rhs.W + lhs.Y * rhs.Z - lhs.Z * rhs.Y,
				- lhs.X * rhs.Z + lhs.Y * rhs.W + lhs.Z * rhs.X,
				  lhs.X * rhs.Y - lhs.Y * rhs.X - lhs.Z * rhs.W);// Is this an orientation?
		}
	}
}
