#pragma once
#include <UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp>
#include <UCodeRunTime/ULibrarys/AssetManagement/AssetPtr.hpp>
#include "AssetManager.hpp"
CoreStart
class RawEntityData
{
public:
	inline static const char* FileExt = "UEntity";
	inline static const char* FileExtDot = ".UEntity";
	
	RawEntityData();
	RawEntityData(const UID& id,const UCode::Entity* Entity,USerializerType type);
	RawEntityData(const UID& id,const UCode::Scene2dData::Entity_Data& Entity);

	UCodeLangNoDiscard static bool WriteToFile(const Path&  Path,const RawEntityData& Data,USerializerType Type);
	UCodeLangNoDiscard static bool ReadFromFile(const Path&  Path,RawEntityData& Data);
	static void ToBits(USerializer& output, const RawEntityData& Data);
	static void FromBits(UDeserializer& input, RawEntityData& Data);


	inline const auto& Get_Value() { return _Data; }
	UID _UID;
	UCode::Scene2dData::Entity_Data _Data;
};

class RawEntityDataAsset : public Asset
{
public:
	
	RawEntityDataAsset(){

	}
	RawEntityDataAsset(RawEntityData&& asset)
		:_Base(std::move(asset))
	{

	}
	RawEntityData _Base;

	ManagedPtr<RawEntityDataAsset> GetManaged()
	{
		auto v =this->Get_Managed();
		return *(ManagedPtr<RawEntityDataAsset>*)&v;
	}
};
using RawEntityDataAssetPtr = ManagedPtr<RawEntityDataAsset>;
using RawEntityDataPtr= AssetPtr<RawEntityDataAsset,RawEntityData>;
CoreEnd

