#pragma once

#include <windows.h>

#include <xnamath.h>

namespace Rorn
{
	namespace Engine
	{
		class Light
		{
		public:
			Light(const XMFLOAT4& direction, const XMFLOAT4& color);

			XMFLOAT4 Direction;
			XMFLOAT4 Color;
		};
	}
}