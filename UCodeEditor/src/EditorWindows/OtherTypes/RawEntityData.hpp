#pragma once
#include <UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp>
#include <Editor/EditorNamespace.hpp>
#include <Typedefs.hpp>
EditorStart
class RawEntityData
{
public:
	inline static const char* FileExt = "UEntity";
	inline static const char* FileExtDot = ".UEntity";
	
	RawEntityData();
	RawEntityData(const UCode::Entity* Entity);
	RawEntityData(const UCode::Scene2dData::Entity_Data& Entity);

	UCODE_EDITOR_NODISCARD static bool WriteToFile(const Path&  Path,const RawEntityData& Data);
	UCODE_EDITOR_NODISCARD static bool ReadFromFile(const Path&  Path, RawEntityData& Data);

	inline const auto& Get_Value() { return _Data; }
private:
	UCode::Scene2dData::Entity_Data _Data;
};
EditorEnd

