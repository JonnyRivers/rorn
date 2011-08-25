#pragma once

#ifdef _PROFILER
	#define CREATE_PROFILER_TASK(taskName) ProfilerTaskStackHelper profilerTask(taskName)
	#define CREATE_PROFILER_TASK_DESC(taskName, description) ProfilerTaskStackHelper profilerTask(taskName, description)
	#define CREATE_NAMED_PROFILER_TASK(objectName, taskName) ProfilerTaskStackHelper objectName(taskName)
	#define CREATE_NAMED_PROFILER_TASK_DESC(objectName, taskName, description) ProfilerTaskStackHelper objectName(taskName, description)
#else
	#define CREATE_PROFILER_TASK(taskName) 0
	#define CREATE_PROFILER_TASK_DESC(taskName, description) 0
	#define CREATE_NAMED_PROFILER_TASK(objectName, taskName) 0
	#define CREATE_NAMED_PROFILER_TASK_DESC(objectName, taskName, description) 0
#endif

#ifdef _PROFILER

namespace caspian 
{
	class ProfilerTaskStackHelper
	{
	public:
		ProfilerTaskStackHelper(const char* const taskName);
		ProfilerTaskStackHelper(const char* const taskName, const char* const description);
		~ProfilerTaskStackHelper();
	private:
		ProfilerTaskStackHelper(const ProfilerTaskStackHelper&);
		ProfilerTaskStackHelper& operator=(const ProfilerTaskStackHelper&);
	};
}

#endif// _PROFILER