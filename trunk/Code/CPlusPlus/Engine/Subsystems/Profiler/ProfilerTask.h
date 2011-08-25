#pragma once

#ifdef _PROFILER

#include <fstream>

namespace caspian 
{
	class ProfilerTask
	{
	public:
		ProfilerTask(int taskIndex, const char* const taskName, const char* description, int64 startTime, int stackLevel);
		void Stop(int64 endTime);

		friend std::ostream& operator<<(std::ostream& stream, const ProfilerTask& task);
	private:
		int taskIndex_;
		char name_[104];// fixed size to prevent runtime allocations
		char description_[128];// fixed size to prevent runtime allocations
		int64 startTime_;
		int64 endTime_;
		int stackLevel_;
	};
}

#endif// _PROFILER