#pragma once

#include "../../../../Shared/RornMaths/Direction.h"
#include "../../../../Shared/RornMaths/EulerAngles.h"
#include "../../../../Shared/RornMaths/Position.h"

#include "../../../Interfaces/IFreeCamera.h"

#include "Camera.h"

namespace Rorn
{
	namespace Engine
	{
		class FreeCamera : public Camera, IFreeCamera
		{
		public:
			FreeCamera(const Rorn::Maths::Position& position, const Rorn::Maths::EulerAngles& eulerAngles);
			~FreeCamera();

			virtual void AlterHeading(float deltaHeading);
			virtual void AlterPitch(float deltaPitch);
			virtual void AlterBank(float deltaBank);
			virtual void Translate(const Maths::Direction& translation);

			virtual Rorn::Maths::Matrix4x4 BuildWorldToViewMatrix() const;
		private:
			FreeCamera(FreeCamera&);
			FreeCamera& operator=(FreeCamera&);

			Rorn::Maths::Position position_;
			Rorn::Maths::EulerAngles eulerAngles_;
		};
	}
}