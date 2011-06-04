#pragma once

namespace Rorn
{
	namespace Engine
	{
		struct IMouse
		{
		public:
			virtual bool IsLeftButtonDown() const = 0;
			virtual bool IsRightButtonDown() const = 0;
			virtual bool IsWheelButtonDown() const = 0;
			virtual long GetXMovement() const = 0;
			virtual long GetYMovement() const = 0;
			virtual long GetZMovement() const = 0;
		};
	}
}