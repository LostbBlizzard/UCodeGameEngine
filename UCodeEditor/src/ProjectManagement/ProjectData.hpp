#pragma once


#include <UCodeRunTime/BasicTypes.hpp>
#include <Editor/EditorNamespace.hpp>
#include <Typedefs.hpp>
#include <Serialization.hpp>
#include "Plugin/UPlugin.hpp"
EditorStart
class ProjectData
{
public:
	String _ProjectName;
	String _CompanyName;
	Version _Version;

	USerializerType _SerializeType= USerializerType::Default;
	
	UID StartScene;
	Vector<UPluginID> _Plugins;


	inline static const char* FileName = "UProjectData.data";

	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path& Path, const ProjectData& data, USerializerType type= USerializerType::Default);
	static void ToBits(USerializer& output, const ProjectData& data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const Path& Path, ProjectData& Out);
	static void FromBits(UDeserializer& input, ProjectData& Out);
};
EditorEnd

