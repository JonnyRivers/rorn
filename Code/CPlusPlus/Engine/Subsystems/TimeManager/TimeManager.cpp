#include "TimeManager.h"

using namespace Rorn::Engine;

/*static*/ TimeManager& TimeManager::instance_ = TimeManager();// init static instance

/*static*/ TimeManager& TimeManager::GetInstance()
{
	return instance_;
}

TimeManager::TimeManager(void)
	: firstStep_(true)
{
	
}

void TimeManager::Step(float& timePassed)
{
	if(!firstStep_)
	{
		LARGE_INTEGER timeAtThisStep;
		::QueryPerformanceCounter(&timeAtThisStep);
		timePassed = static_cast<float>(timeAtThisStep.QuadPart - timeAtLastStep_.QuadPart) /
			static_cast<float>(timerFrequency_.QuadPart);
		timeAtLastStep_ = timeAtThisStep;

		// check for debugger induced massive frame times
		if(timePassed > 0.1f)
			timePassed = 0.1f;
	}
	else
	{
		::QueryPerformanceFrequency(&timerFrequency_);
		::QueryPerformanceCounter(&timeAtStart_);
		timeAtLastStep_ = timeAtStart_;

		firstStep_ = false;
		timePassed = 0.0f;
	}
}