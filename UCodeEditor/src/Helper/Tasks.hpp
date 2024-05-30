#pragma once
#include "Editor/EditorNamespace.hpp"
#include "Typedefs.hpp"
#include <mutex>
EditorStart
struct RuningTask
{

	enum class Type :u16
	{
		null,
		BuildingShaders,
		BuildingUCodeFiles,
		BuildingProject,
	};

	Type TaskType;

	//0-100 number
	Byte percentage = 30;
	String Status;
};

//Thread Stadt
struct RuningTasksInfo
{
	static bool HasTaskRuning(RuningTask::Type Type);

	static void SetTaskStatus(RuningTask::Type Type,String Status,Byte percentage);
	static void AddTask(const RuningTask& Value);
	static void ReMoveTask(RuningTask::Type Type);
	static Vector<RuningTask>& Get_Tasks();
private:
	inline static Vector<RuningTask> _Tasks;
};
EditorEnd

