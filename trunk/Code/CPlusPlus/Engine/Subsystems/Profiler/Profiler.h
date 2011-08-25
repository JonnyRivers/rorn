#pragma once

#ifdef _PROFILER

#include <list>
#include <stack>

#include "Framework/Singleton.h"

#include "ProfilerTask.h"

namespace caspian 
{
	class Profiler : public Singleton<Profiler>
	{
	public:
		Profiler();

		void StartFrame();
		void EndFrame();

		void PushProfilerTask(const char* const taskName, const char* const description);
		void PopProfilerTask();
	private:
		static int64 GetCurrentTime();
		static int64 GetTimerFrequency();

		unsigned int taskIndex_;
		unsigned int frameIndex_;
		std::stack<ProfilerTask> tasksInProgress_;
		std::vector<ProfilerTask> completedTasks_;
	};
}

#endif// _PROFILER