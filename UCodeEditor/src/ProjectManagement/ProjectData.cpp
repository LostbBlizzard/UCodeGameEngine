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
	output.Write("_SerializeType", *(USerializerType_t*)&data._SerializeType);
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
	input.ReadType("_SerializeType", *(USerializerType_t*)&Out._SerializeType, *(USerializerType_t*)&Out._SerializeType);
}
EditorEnd