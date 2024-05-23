#pragma once
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
#include "Typedefs.hpp"
#include "Serialization.hpp"
EditorStart

struct TilePalette
{
	UID _UID;

	inline static const char* FileExt = "UTilePalette";
	inline static const char* FileExtDot = ".UTilePalette";
	
	void PushData(USerializer& node) const;
	static bool FromString(TilePalette& out, UDeserializer& text);

	static bool FromFile(TilePalette& out, const Path& Path);
	static bool ToFile(const Path& path,const TilePalette& data, USerializerType Type);
};
struct TilePaletteAsset : UC::Asset
{
public:
	TilePalette _Base;
	TilePaletteAsset()
	{

	}
	TilePaletteAsset(TilePalette&& base)
		:_Base(std::move(base))
	{

	}

	ManagedPtr<TilePaletteAsset> GetManaged()
	{
		auto V = this->Get_Managed();
		return *(ManagedPtr<TilePaletteAsset>*)&V;
	}
};
using TilePaletteAssetPtr = ManagedPtr<TilePaletteAsset>;
using TilePalettePtr = UC::AssetPtr<TilePaletteAsset,TilePalette>;
EditorEnd
