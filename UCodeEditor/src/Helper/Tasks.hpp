#pragma once
#include "Editor/EditorNamespace.hpp"
#include "Typedefs.hpp"
#include <mutex>
EditorStart
struct RuningTask
{

	enum class Type : UInt8
	{
		null,
		BuildingShaders,
		BuildingUCodeFiles,
		BuildingProject,
	};

	Type TaskType;

};

//Thread Stadt
struct RuningTasksInfo
{
	static bool HasTaskRuning(RuningTask::Type Type);

	static void AddTask(const RuningTask& Value);
	static void ReMoveTask(RuningTask::Type Type);
	static Vector<RuningTask>& Get_Tasks();
private:
	inline static Vector<RuningTask> _Tasks;
};
EditorEnd

