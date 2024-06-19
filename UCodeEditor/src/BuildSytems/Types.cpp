#include "Types.hpp"
#include "ProjectManagement/ProjectManger.hpp"

EditorStart
void BuildSetings::SetProject(const Path& ProjectDir)
{
	_InputDir = ProjectDir / "Assets";
	auto cacheddir = ProjectManger::GetProjectCachedDir(ProjectDir);

	_OutDir = cacheddir / "output";
	TemporaryPlatfromPath = cacheddir / "build";
	TemporaryGlobalPath = cacheddir / "build" / "global";
}

EditorEnd
