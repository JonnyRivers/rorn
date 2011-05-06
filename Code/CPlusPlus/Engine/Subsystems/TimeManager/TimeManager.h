#pragma once

#include <deque>

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
			float GetFPS() const;
			void SleepTillNextFrame() const;
		private:
			static TimeManager& instance_;

			TimeManager(void);

			bool firstStep_;
			LARGE_INTEGER timerFrequency_;
			LARGE_INTEGER timeAtStart_;
			LARGE_INTEGER timeAtLastStep_;

			static const int maxFramesRecorded_ = 200;
			std::deque<float> previousFrameTimes_;
		};
	}
}