#include "Tasks.hpp"
#include "UCodeRunTime/CoreSystems/Threads.hpp"
EditorStart
bool RuningTasksInfo::HasTaskRuning(RuningTask::Type Type)
{
	UCode::Threads::ThrowErrIfNotOnMainThread();

	for (const auto& Items : _Tasks)
	{
		if (Items.TaskType == Type)
		{
			return true;
		}
	}
	return false;
}

void RuningTasksInfo::SetTaskStatus(RuningTask::Type Type, String Status, Byte percentage)
{
	UCode::Threads::ThrowErrIfNotOnMainThread();

	for (auto& Items : _Tasks)
	{
		if (Items.TaskType == Type)
		{
			Items.Status = Status;
			Items.percentage = percentage;
		}
	}
}

void RuningTasksInfo::AddTask(const RuningTask& Value)
{
	UCode::Threads::ThrowErrIfNotOnMainThread();

	_Tasks.push_back(Value);
}

void RuningTasksInfo::ReMoveTask(RuningTask::Type Type)
{
	UCode::Threads::ThrowErrIfNotOnMainThread();

	for (auto it = _Tasks.begin(); it != _Tasks.end(); it++)
	{
		const auto& Task = *it;
		if (Task.TaskType == Type)
		{
			_Tasks.erase(it);
			break;
		}
	}
}
Vector<RuningTask>& RuningTasksInfo::Get_Tasks()
{
	UCode::Threads::ThrowErrIfNotOnMainThread();
	return _Tasks;
}
EditorEnd