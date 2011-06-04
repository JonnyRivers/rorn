#pragma once

namespace Rorn
{
	namespace Engine
	{
		struct IKeyboard
		{
		public:
			virtual bool IsKeyDown(int key) const = 0;
		};
	}
}