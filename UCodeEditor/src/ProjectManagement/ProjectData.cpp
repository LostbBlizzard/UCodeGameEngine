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
	output.Write("_GameIcon", data.GameIcon);
	output.Write("_Plugins", data._Plugins);
	output.Write("_AssetsToKeepLoaded", data._AssetsToKeepLoaded);
	output.Write("_ModingSupport", data.ModingSupport);
	output.Write("_PrefabScene", data.PrefabScene);
	output.Write("_PrefabUIScene", data.PrefabUIScene);
	output.Write("_EditorOnlyFolders", data.EditorOnlyFolders);
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
	input.ReadType("_StartScene", Out.StartScene, Out.StartScene);
	if (input.HasField("_GameIcon"))
	{
		input.ReadType("_GameIcon", Out.GameIcon, Out.GameIcon);
	}
	if (input.HasField("_Plugins"))
	{
		input.ReadType("_Plugins", Out._Plugins, Out._Plugins);
	}
	if (input.HasField("_AssetsToKeepLoaded"))
	{
		input.ReadType("_AssetsToKeepLoaded", Out._AssetsToKeepLoaded, Out._AssetsToKeepLoaded);
	}
	if (input.HasField("_ModingSupport"))
	{
		input.ReadType("_ModingSupport", Out.ModingSupport, Out.ModingSupport);
	}
	if (input.HasField("_PrefabScene"))
	{
		input.ReadType("_PrefabScene", Out.PrefabScene, Out.PrefabScene);
	}
	if (input.HasField("_PrefabUIScene"))
	{
		input.ReadType("_PrefabUIScene", Out.PrefabUIScene, Out.PrefabUIScene);
	}
	if (input.HasField("_EditorOnlyFolders"))
	{
		input.ReadType("_EditorOnlyFolders", Out.EditorOnlyFolders, Out.EditorOnlyFolders);
	}
}
EditorEnd