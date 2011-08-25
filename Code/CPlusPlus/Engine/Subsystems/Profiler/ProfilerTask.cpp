#include "stdafx.h"

#ifdef _PROFILER

#include "Profiler/ProfilerTask.h"

namespace caspian
{
	ProfilerTask::ProfilerTask(int taskIndex, const char* const taskName, const char* description, int64 startTime, int stackLevel) 
		: taskIndex_(taskIndex), startTime_(startTime), endTime_(startTime), stackLevel_(stackLevel)
	{
		strncpy(name_, taskName, sizeof(name_));

		if( description != NULL )
			strncpy(description_, description, sizeof(description_));
		else
			description_[0] = '\0';
	}

	void ProfilerTask::Stop(int64 endTime)
	{
		endTime_ = endTime;
	}

	/*friend*/ std::ostream& operator<<(std::ostream& stream, const ProfilerTask& task)
	{
		stream.write(reinterpret_cast<const char*>(&task.taskIndex_), sizeof(task.taskIndex_));

		unsigned int nameLength = strlen(task.name_);
		stream.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
		stream.write(task.name_, nameLength);

		unsigned int descriptionLength = strlen(task.description_);
		stream.write(reinterpret_cast<const char*>(&descriptionLength), sizeof(descriptionLength));
		stream.write(task.description_, descriptionLength);
		
		stream.write(reinterpret_cast<const char*>(&task.startTime_), sizeof(task.startTime_));
		stream.write(reinterpret_cast<const char*>(&task.endTime_), sizeof(task.endTime_));
		stream.write(reinterpret_cast<const char*>(&task.stackLevel_), sizeof(task.stackLevel_));

		return stream;
	}
}

#endif// _PROFILER