#pragma once
#include "Types.hpp"
#include "Serialization.hpp"
#include "UEditorModules/UEditorModule.hpp"
EditorStart
class BuildSytemManger
{
public:	
	using BuildRet = Result<Path, ExportErrors>;
	BuildSetings Setings;
	BuildSytemManger::BuildRet BuildProject();
	void Reset();

	struct BuildError
	{
		String MSG;
	};
	Vector<BuildError> errors;
private:
	BuildRet BuildProjectGameData(const Path& GameFilesDataPath, USerializerType SerializerMode);
	Path Platfromteppath;

	//Main
	BuildRet Build(const WindowsBuildSetings& setings);
	BuildRet Build(const LinuxBuildSetings& setings);
	BuildRet Build(const MacOsBuildSetings& setings);


	//Secondary platform
	BuildRet Build(const WebBuildSetings& setings);
	BuildRet Build(const AndroidBuildSetings& setings);
	BuildRet Build(const IOSBuildSetings& setings);


	//Consoles,VR and whatever in the future will use the CustomBuildSetings
	BuildRet Build(const CustomBuildSetings& setings);


	ExportSettings Settings;
};
EditorEnd

