#pragma once

#include "PhysicsRunTime2d.hpp"
PhysicsStart
class TileMapCollider2d :public PhysicsObject2d
{
private:
	void Start() override;
public:
	using IndexType = unsigned int;//need to be same as TileMapRenderer
	struct Tile {};

	TileMapCollider2d(Entity* entity, IndexType MapX, IndexType MapY);
	~TileMapCollider2d();


	inline Tile* Get_Tile(IndexType x, IndexType y)
	{
		auto numberofcolums = _TileMapSizeY;
		auto index = y * numberofcolums + x;
#ifdef DEBUG
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
#ifdef DEBUG
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
PhysicsEnd


