#pragma once

namespace Rorn
{
	namespace Engine
	{
		struct IModelInstance
		{
		public:
			virtual void RotateY(float angle) = 0;
		};
	}
}