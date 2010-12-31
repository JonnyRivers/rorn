#pragma once

#include <windows.h>

namespace Rorn
{
	namespace Engine
	{
		class TimeManager
		{
		public:
			static TimeManager& GetInstance();

			void Step(float& timePassed);
		private:
			static TimeManager& instance_;

			TimeManager(void);

			bool firstStep_;
			LARGE_INTEGER timerFrequency_;
			LARGE_INTEGER timeAtStart_;
			LARGE_INTEGER timeAtLastStep_;
		};
	}
}