#pragma once

#include "PCTimekeeper.h"

using namespace Rorn::Engine;

namespace
{
	int maxFramesRecorded_ = 200;
	float maxSimulatedFrameTime_ = 0.1f;
}

PCTimekeeper::PCTimekeeper(IDiagnostics* diagnostics) : diagnostics_(diagnostics), firstStep_(true)
{

}

PCTimekeeper::~PCTimekeeper()
{
}

float PCTimekeeper::GetTimeElapsedSinceLastRequest()
{
	float timeElapsed;
	if(!firstStep_)
	{
		LARGE_INTEGER timeAtThisStep;
		::QueryPerformanceCounter(&timeAtThisStep);
		timeElapsed = static_cast<float>(timeAtThisStep.QuadPart - timeAtLastStep_.QuadPart) /
			static_cast<float>(timerFrequency_.QuadPart);
		timeAtLastStep_ = timeAtThisStep;

		// check for debugger induced massive frame times
		if(timeElapsed > maxSimulatedFrameTime_)
			timeElapsed = maxSimulatedFrameTime_;

		// update previous frames cache
		if(previousFrameTimes_.size() == maxFramesRecorded_)
			previousFrameTimes_.pop_front();
		previousFrameTimes_.push_back(timeElapsed);
	}
	else
	{
		::QueryPerformanceFrequency(&timerFrequency_);
		::QueryPerformanceCounter(&timeAtStart_);
		timeAtLastStep_ = timeAtStart_;

		firstStep_ = false;
		timeElapsed = 0.0f;
	}

	return timeElapsed;
}

float PCTimekeeper::GetFramerate() const
{
	if(previousFrameTimes_.empty())
		return 0.0f;

	float totalFrameTimes = 0.0f;
	std::deque<float>::const_iterator frameTimeIter;
	for(frameTimeIter = previousFrameTimes_.cbegin() ; frameTimeIter != previousFrameTimes_.cend() ; ++frameTimeIter)
		totalFrameTimes += *frameTimeIter;

	float framerate = static_cast<float>(previousFrameTimes_.size()) / totalFrameTimes;

	return framerate;
}

void PCTimekeeper::SleepTillNextFrame() const
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