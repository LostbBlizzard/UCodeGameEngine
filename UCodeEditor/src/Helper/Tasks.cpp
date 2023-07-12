#include "Tasks.hpp"
#include "UCodeRunTime/CoreBooks/BookOfThreads.hpp"
EditorStart
bool RuningTasksInfo::HasTaskRuning(RuningTask::Type Type)
{
	UCode::BookOfThreads::ThrowErrIfNotOnMainThread();

	for (const auto& Items : _Tasks)
	{
		if (Items.TaskType == Type)
		{
			return true;
		}
	}
	return false;
}

void RuningTasksInfo::AddTask(const RuningTask& Value)
{
	UCode::BookOfThreads::ThrowErrIfNotOnMainThread();

	_Tasks.push_back(Value);
}

void RuningTasksInfo::ReMoveTask(RuningTask::Type Type)
{
	UCode::BookOfThreads::ThrowErrIfNotOnMainThread();

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
	UCode::BookOfThreads::ThrowErrIfNotOnMainThread();
	return _Tasks;
}
EditorEnd