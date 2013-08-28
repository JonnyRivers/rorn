#pragma once

#include "../../../../Shared/RornMaths/EulerAngles.h"
#include "../../../../Shared/RornMaths/Vector4.h"

#include "../../../Interfaces/IFreeCamera.h"

#include "Camera.h"

namespace Rorn
{
	namespace Engine
	{
		class FreeCamera : public Camera, IFreeCamera
		{
		public:
			FreeCamera(const Rorn::Maths::Vector4& position, const Rorn::Maths::EulerAngles& eulerAngles);
			~FreeCamera();

			virtual void GetPosition(Maths::Vector4& position);
			virtual void GetEyeDirection(Maths::Vector4& eyeDirection);
			virtual void GetUpVector(Maths::Vector4& up);
			virtual void GetRightVector(Maths::Vector4& right);

			virtual void AlterHeading(float deltaHeading);
			virtual void AlterPitch(float deltaPitch);
			virtual void AlterBank(float deltaBank);
			virtual void Translate(const Maths::Vector4& translation);

			virtual Rorn::Maths::Matrix4x4 BuildWorldToViewMatrix() const;
		private:
			FreeCamera(FreeCamera&);
			FreeCamera& operator=(FreeCamera&);

			Rorn::Maths::Vector4 position_;
			Rorn::Maths::EulerAngles eulerAngles_;
		};
	}
}