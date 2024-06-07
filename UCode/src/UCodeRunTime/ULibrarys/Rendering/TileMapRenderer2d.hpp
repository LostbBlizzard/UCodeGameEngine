#pragma once

#include "RenderRunTime2d.hpp"

#include "../AssetManagement/AssetRendering.hpp"
RenderingStart

struct TileData
{
	inline static const char* FileExt = "UTile";
	inline static const char* FileExtDot = ".UTile";

	SpritePtr Sprite;
	Color Color;

	void PushData(USerializer& node) const;
	static bool FromString(TileData& out, UDeserializer& text);

	static bool FromFile(TileData& out, const Path& Path);
	static bool ToFile(const Path& path,const SpriteData& data, USerializerType Type);

};
struct TileAsset : public Asset
{
public:
	TileAsset() 
	{

	}
	TileAsset(TileData&& base) :_Base(std::move(base))
	{

	}
	TileData _Base;

	ManagedPtr<TileAsset> GetManaged()
	{
		auto v =this->Get_Managed();

		return *(ManagedPtr<TileAsset>*)&v;
	}
};
using TileAssetPtr = ManagedPtr<TileAsset>;
using TilePtr =	AssetPtr<TileAsset,TileData>;


class TileMapRenderer final : public Renderer2d
{
private:
	void Start() override;
	void OnDraw() override;

public:
	struct Tile
	{
		TilePtr tile;
		Color color;	
		
		int xoffset = 0;
		int yoffset = 0;
	};
	TileMapRenderer(Entity* entity);
	~TileMapRenderer() override;

	Bounds2d Get_Bounds() const;

	void Serialize(USerializer& Serializer)  const override;

	void Deserialize(UDeserializer& Serializer) override;

	rttr::instance Get_Rttr_Instance() override { return rttr::instance(this); }

	NullablePtr<Tile> Get_Tile(int x, int y);
	void AddTile(Tile& tile);
	void AddTileRaw(Tile& tile);
	void RemoveTile(int x,int y);

	Vec2 GetTilePos(Vec2 pos);

	Color color;
	ShaderPtr shader;
	RenderRunTime2d::DrawLayer_t DrawLayer = {};
	RenderRunTime2d::DrawOrder_t DrawOrder = {};
	bool Showgrid = false;
	
	static UComponentsID Get_TypeID();	
	static UComponentData type_Data;
private:
	Vector<Tile> _Tiles;
	void DrawGrid();
};
RenderingEnd
