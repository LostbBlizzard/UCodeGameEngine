#pragma once

#include "RenderRunTime2d.hpp"

#include "GpuTypes/Texture.hpp"
#include "GpuTypes/Sprite.hpp"
RenderingStart
class TileMapRenderer final : public Renderer2d
{
private:
	void Start() override;
	void OnDraw() override;

public:
	using IndexType = unsigned int;
	struct Tile
	{
		Sprite* texture;
		Color color;
		Tile() :texture(nullptr)
		{
		}
	};
	TileMapRenderer(Entity* entity, IndexType MapX, IndexType MapY);
	~TileMapRenderer() override;

public:
	Bounds2d Get_Bounds() const;
	Color color;
	Shader* shader;

	RenderRunTime2d::DrawLayer_t DrawLayer;
	RenderRunTime2d::DrawOrder_t DrawOrder;
	inline Tile* Get_Tile(IndexType x, IndexType y)
	{
		auto numberofcolums = _TileMapSizeY;
		auto index = y * numberofcolums + x;
#if UCodeGEDebug
		//if (TilesData.isvaldIndex(index))
		{
			return &TilesData[index];
		}
		//else
		{
			return nullptr;
		}
#else
		return &TilesData[index];
#endif // DEBUG
	}

	inline Tile* Get_Tile(size_t index)
	{
#if UCodeGEDebug
		//if (TilesData.isvaldIndex(index))
		{
			return &TilesData[index];
		}
		//else
		{
			return nullptr;
		}
#else
		return &TilesData[index];
#endif // DEBUG
	}

	inline IndexType Get_TileMapSizeX() const { return _TileMapSizeX; }
	inline IndexType Get_TileMapSizeY() const { return _TileMapSizeY; }
	inline IndexType Get_MaxTiles() const { return _TileMapSizeX * _TileMapSizeY; }
private:
	IndexType _TileMapSizeX;
	IndexType _TileMapSizeY;
	Vector<Tile> TilesData;
	static UComponentData type_Data;
};
RenderingEnd
