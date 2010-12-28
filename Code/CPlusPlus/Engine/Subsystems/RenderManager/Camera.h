#pragma once

#include <Windows.h>
#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		class Camera
		{
		public:
			Camera(XMVECTOR eye, XMVECTOR target, XMVECTOR up);

			XMVECTOR Eye;
			XMVECTOR Target;
			XMVECTOR Up;
		};
	}
}