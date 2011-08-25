#include "StdAFX.h"

#ifdef _PROFILER

#ifdef _IOS
#include <mach/mach.h>
#include <mach/mach_time.h>
#else
#include <windows.h>
#endif

#include <fstream>

#include "Profiler/Profiler.h"
#include "File/FileManager.h"

namespace caspian
{
	Profiler::Profiler() : frameIndex_(0)
	{
		completedTasks_.reserve(4096);// reserve lots of space to prevent run-time allocations
	}

	void Profiler::StartFrame()
	{
		assert(tasksInProgress_.empty());

		while(!tasksInProgress_.empty())
			tasksInProgress_.pop();

		completedTasks_.clear();

		taskIndex_ = 0;
	}
	
	void Profiler::EndFrame()
	{
		assert(tasksInProgress_.empty());

		// flush it all to disk
		std::ofstream profileLogStream;
		
		if( frameIndex_ == 0 )
		{
			String outPath = FileManager::Get()->GetFullPath( "run.profilelog" );
			profileLogStream.open( outPath.c_str(), ios_base::out | ios_base::binary );

			int64 timerFrequency = GetTimerFrequency();
			profileLogStream.write(reinterpret_cast<const char*>(&timerFrequency), sizeof(timerFrequency));
		}
		else
		{
			String outPath = FileManager::Get()->GetFullPath( "run.profilelog" );
			profileLogStream.open( outPath.c_str(), ios_base::app | ios_base::out | ios_base::binary );
		}
		
		if(profileLogStream.is_open())
		{
			profileLogStream.write(reinterpret_cast<const char*>(&frameIndex_), sizeof(frameIndex_));
			unsigned int numTasks = completedTasks_.size();
			profileLogStream.write(reinterpret_cast<const char*>(&numTasks), sizeof(numTasks));
			for(std::vector<ProfilerTask>::const_iterator taskIter = completedTasks_.begin() ; taskIter != completedTasks_.end() ; ++taskIter)
			{
				profileLogStream << *taskIter;
			}
			profileLogStream.close();
		}

		++frameIndex_;
	}

	void Profiler::PushProfilerTask(const char* const taskName, const char* const description)
	{
		// construct and push the profile task
		int64 startTime = GetCurrentTime();
		tasksInProgress_.push(ProfilerTask(taskIndex_++, taskName, description, startTime, tasksInProgress_.size()));
	}

	void Profiler::PopProfilerTask()
	{
		// time the end, update the task then add it to the completed tasks collection and pop the in progress stack
		int64 endTime = GetCurrentTime();
		ProfilerTask& topTask = tasksInProgress_.top();
		topTask.Stop(endTime);
		completedTasks_.push_back(topTask);
		tasksInProgress_.pop();
	}

	/*static*/ int64 Profiler::GetCurrentTime()
	{
#if _IOS
		return static_cast<int64>(mach_absolute_time());
#else
		LARGE_INTEGER currentTime;
		::QueryPerformanceCounter(&currentTime);
		return currentTime.QuadPart;
#endif
	}

	/*static*/ int64 Profiler::GetTimerFrequency()
	{
#if _IOS
		mach_timebase_info_data_t info;
		mach_timebase_info(&info);
		return 1000000000 * info.numer / info.denom;
#else
		LARGE_INTEGER performanceFrequency;
		::QueryPerformanceFrequency(&performanceFrequency);
		return performanceFrequency.QuadPart;
#endif
	}
}

#endif// _PROFILER