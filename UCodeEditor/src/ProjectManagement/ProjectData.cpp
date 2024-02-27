#include "ProjectData.hpp"
#include <fstream>
#include <UCodeRunTime/ULibrarys/Serialization_Library.h>

EditorStart



bool ProjectData::WriteToFile(const Path& Path, const ProjectData& data, USerializerType type)
{
	auto File = std::ofstream(Path);
	if (File.is_open())
	{
		USerializer output(type);
		ToBits(output, data);
	
		//
		output.ToStream(File, true);

		File.close();
		return true;
	}
	return false;
}

void ProjectData::ToBits(USerializer& output, const ProjectData& data)
{
	output.Write("_ProjectName", data._ProjectName);
	output.Write("_CompanyName", data._CompanyName);
	output.Write("_Verson", data._Version);
	
	output.Write("_SerializeType", *(USerializerType_t*)&data._SerializeType);
	output.Write("_StartScene", data.StartScene);
	output.Write("_Plugins", data._Plugins);
	output.Write("_AssetsToKeepLoaded", data._AssetsToKeepLoaded);
}

bool ProjectData::ReadFromFile(const Path& Path, ProjectData& Out)
{
	UDeserializer input;
	if (!UDeserializer::FromFile(Path, input))
	{
		return false;
	}
	FromBits(input, Out);

	return true;
}
void ProjectData::FromBits(UDeserializer& input, ProjectData& Out)
{
	input.ReadType("_ProjectName", Out._ProjectName, Out._ProjectName);
	input.ReadType("_CompanyName", Out._CompanyName, Out._CompanyName);
	
	if (input.HasField("_Verson"))
	{
		input.ReadType("_Verson", Out._Version, Out._Version);
	}
	

	input.ReadType("_SerializeType", *(USerializerType_t*)&Out._SerializeType, *(USerializerType_t*)&Out._SerializeType);
	input.ReadType("_StartScene",Out.StartScene, Out.StartScene);

	if (input.HasField("_Plugins"))
	{
		input.ReadType("_Plugins", Out._Plugins, Out._Plugins);
	}
	if (input.HasField("_AssetsToKeepLoaded"))
	{
		input.ReadType("_AssetsToKeepLoaded", Out._AssetsToKeepLoaded, Out._AssetsToKeepLoaded);
	}
}
EditorEnd