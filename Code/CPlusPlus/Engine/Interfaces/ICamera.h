#pragma once

#include "../../Shared/RornMaths/Vector4.h"

namespace Rorn
{
	namespace Engine
	{
		struct ICamera
		{
		public:
			virtual void AlterHeading(float deltaHeading) = 0;
			virtual void AlterPitch(float deltaPitch) = 0;
			virtual void AlterBank(float deltaBank) = 0;
			virtual void Translate(const Maths::Vector4& translation) = 0;
		};
	}
}
