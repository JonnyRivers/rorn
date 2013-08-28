#pragma once

#include "../../Shared/RornMaths/Vector4.h"

namespace Rorn
{
	namespace Engine
	{
		struct ICamera
		{
		public:
			virtual void GetPosition(Maths::Vector4& position) = 0;
			virtual void GetEyeDirection(Maths::Vector4& eyeDirection) = 0;
			virtual void GetUpVector(Maths::Vector4& up) = 0;
			virtual void GetRightVector(Maths::Vector4& right) = 0;

			virtual void AlterHeading(float deltaHeading) = 0;
			virtual void AlterPitch(float deltaPitch) = 0;
			virtual void AlterBank(float deltaBank) = 0;
			virtual void Translate(const Maths::Vector4& translation) = 0;
		};
	}
}
