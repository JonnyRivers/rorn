#pragma once

#include "../../../../Shared/RornMaths/Float2.h"
#include "../../../../Shared/RornMaths/Float4.h"

namespace Rorn
{
	namespace Engine
	{
		struct BlitVertexFormat
		{
			Maths::Float4 Position;
			Maths::Float2 DiffuseUV;
		};
	}
}
