#pragma once
#include "Types.hpp"
#include "Serialization.hpp"
EditorStart
class BuildSytemManger
{
public:	
	BuildSetings Setings;
	bool BuildProject();
	void Reset();

	struct BuildError
	{
		String MSG;
	};
	Vector<BuildError> errors;
private:
	bool BuildProjectGlobalWasGameData(const Path& GameFilesDataPath, USerializerType SerializerMode);
	

	//Main
	bool Build(const WindowsBuildSetings& setings);
	bool Build(const LinuxBuildSetings& setings);
	bool Build(const MacOsBuildSetings& setings);


	//Secondary platform
	bool Build(const WebBuildSetings& setings);
	bool Build(const AndroidBuildSetings& setings);
	bool Build(const IOSBuildSetings& setings);


	//Consoles,VR and whatever in the future will use the CustomBuildSetings
	bool Build(const CustomBuildSetings& setings);
};
EditorEnd

