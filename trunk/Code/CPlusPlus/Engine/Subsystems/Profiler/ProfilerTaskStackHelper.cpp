#include "stdafx.h"

#ifdef _PROFILER

#include "Profiler/ProfilerTaskStackHelper.h"

#include "Profiler/Profiler.h"

namespace caspian
{
	ProfilerTaskStackHelper::ProfilerTaskStackHelper(const char* const taskName)
	{
		Profiler::Get()->PushProfilerTask(taskName, NULL);
	}

	ProfilerTaskStackHelper::ProfilerTaskStackHelper(const char* const taskName, const char* const description)
	{
		Profiler::Get()->PushProfilerTask(taskName, description);
	}

	ProfilerTaskStackHelper::~ProfilerTaskStackHelper()
	{
		Profiler::Get()->PopProfilerTask();
	}
}

#endif// _PROFILER