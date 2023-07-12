#pragma once
#include "Types.hpp"
#include "Serialization.hpp"
EditorStart
class BuildSytemManger
{
public:	
	BuildSetings Setings;
	void BuildProject();
	void Reset();
private:
	void Build(const WindowsBuildSetings& setings);
	void BuildProjectGlobalWasGameData(const Path& GameFilesDataPath, USerializerType SerializerMode);
	void Build(const WebBuildSetings& setings);
	void Build(const AndroidBuildSetings& setings);
};
EditorEnd

