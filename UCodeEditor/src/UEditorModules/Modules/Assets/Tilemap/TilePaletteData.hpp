#pragma once
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
#include "Typedefs.hpp"
#include "Serialization.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/TileMapRenderer2d.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/SerlizeableType.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/UseUSerializerForYamlAndBit.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/Bit_Implementation/AsssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/Yaml_Implementation/AsssetPtr.hpp"
EditorStart

struct TileData : UC::TileAsset 
{
	UID _UID;
	TileData()
	{

	}
	TileData(const TileData& data)
		: _UID(data._UID)
	{
		this->_Base = data._Base;
		Uid = _UID;
	}
	TileData& operator=(const TileData& data)
	{
		this->_Base = data._Base;
		_UID = data._UID;
		Uid = _UID;
		return *this;
	}
	TileData(TileData&& data) noexcept
		: _UID(data._UID)
	{
		this->_Base = std::move(data._Base);
		this->Uid = _UID;
		data._UID = {};
	}
	TileData& operator=(TileData&& data) noexcept
	{
		this->_Base = std::move(data._Base);
		_UID = data._UID;
		Uid = _UID;
		data._UID = {};
		return *this;
	}

	inline static const char* FileExt = "UTile";
	inline static const char* FileExtDot = ".UTile";
	
	void PushData(USerializer& node) const;
	static bool FromString(TileData& out, UDeserializer& text);

	static bool FromFile(TileData& out, const Path& Path,USerializerType Type);
	static bool ToFile(const Path& path,const TileData& data, USerializerType Type);
};
EditorEnd

UseUSerializerForYamlAndBit(UCodeEditor::TileData);

EditorStart

struct TileDataAsset : TileData
{
public:
	TileDataAsset()
	{

	}
	TileDataAsset(TileData&& base)
		:TileData(std::move(base))
	{

	}

	ManagedPtr<TileDataAsset> GetManaged()
	{
		auto V = this->Get_Managed();
		return *(ManagedPtr<TileDataAsset>*)&V;
	}
};
using TileDataAssetPtr = ManagedPtr<TileDataAsset>;
using TileDataPtr = UC::AssetPtr<TileDataAsset,TileData>;

struct TileDataPack
{
	struct PackTile
	{
		String _Name;
		TileData _Data;

		i32 X = 0;
		i32 Y = 0;
	};
	UC::UID _UID;
	UC::TexturePtr _BaseTexture;
	Vector<PackTile> List;

	inline static const char* FileExt = "UTilePack";
	inline static const char* FileExtDot = ".UTilePack";
	
	void PushData(USerializer& node) const;
	static bool FromString(TileDataPack& out, UDeserializer& text);

	static bool FromFile(TileDataPack& out, const Path& Path, USerializerType Type);
	static bool ToFile(const Path& path,const TileDataPack& data, USerializerType Type);
};


struct TilePalette
{
	struct PaletteItem
	{
		TileDataPtr Ptr;
		i32 X = 0;
		i32 Y = 0;
	};
	UID _UID;
	Vector<PaletteItem> _List;

	inline static const char* FileExt = "UTilePalette";
	inline static const char* FileExtDot = ".UTilePalette";
	
	void PushData(USerializer& node) const;
	static bool FromString(TilePalette& out, UDeserializer& text);

	static bool FromFile(TilePalette& out, const Path& Path, USerializerType Type);
	static bool ToFile(const Path& path,const TilePalette& data, USerializerType Type);
};
EditorEnd

MakeSerlizeType(UCodeEditor::TileDataPack::PackTile,
 Field("_Name", _this->_Name);
 Field("_Data", _this->_Data);
 Field("X", _this->X);
 Field("Y", _this->Y);
);

MakeSerlizeType(UCodeEditor::TilePalette::PaletteItem,
 Field("Tile", _this->Ptr);
 Field("X", _this->X);
 Field("Y", _this->Y);
);
EditorStart
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

struct TilePackAsset : UC::Asset
{
public:
	TileDataPack _Base;
	TilePackAsset()
	{

	}
	TilePackAsset(TileDataPack&& base)
		:_Base(std::move(base))
	{

	}

	ManagedPtr<TilePackAsset> GetManaged()
	{
		auto V = this->Get_Managed();
		return *(ManagedPtr<TilePackAsset>*)&V;
	}
};
using TilePackPtr = ManagedPtr<TilePaletteAsset>;
using TilePackAssetPtr = UC::AssetPtr<TilePaletteAsset,TilePalette>;


EditorEnd
