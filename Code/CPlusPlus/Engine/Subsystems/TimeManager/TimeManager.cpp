#include "TimeManager.h"

#include <algorithm>

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

		// update previous frames cache
		if(previousFrameTimes_.size() == maxFramesRecorded_)
			previousFrameTimes_.pop_front();
		previousFrameTimes_.push_back(timePassed);
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

float TimeManager::GetFPS() const
{
	if(previousFrameTimes_.empty())
		return 0.0f;

	float totalFrameTimes = 0.0f;
	std::deque<float>::const_iterator frameTimeIter;
	for(frameTimeIter = previousFrameTimes_.cbegin() ; frameTimeIter != previousFrameTimes_.cend() ; ++frameTimeIter)
		totalFrameTimes += *frameTimeIter;

	float fps = static_cast<float>(previousFrameTimes_.size()) / totalFrameTimes;

	return fps;
}

void TimeManager::SleepTillNextFrame() const
{
	const float secondsPerFrame_ = 1.0f / 60.0f;
	float timePassed = 0.0f;

	while( timePassed < secondsPerFrame_ )
	{
		LARGE_INTEGER timeAtThisStep;
		::QueryPerformanceCounter(&timeAtThisStep);
		timePassed = static_cast<float>(timeAtThisStep.QuadPart - timeAtLastStep_.QuadPart) / static_cast<float>(timerFrequency_.QuadPart);
	}

}