#pragma once

#include "../../Shared/RornMaths/Vector3.h"

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
			virtual void Translate(const Maths::Vector3& translation) = 0;
		};
	}
}
