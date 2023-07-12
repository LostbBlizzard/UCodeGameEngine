#include "TileMapCollider2d.hpp"
PhysicsStart

UComponentData TileMapCollider2d::type_Data = { "TileMapCollider2d",[](Entity* E) {return (Compoent*)new TileMapCollider2d(E,0,0); } };
TileMapCollider2d::TileMapCollider2d(Entity* entity, IndexType MapX, IndexType MapY) :PhysicsObject2d(entity,&type_Data)
, _TileMapSizeX(MapX), _TileMapSizeY(MapY), TilesData(Get_MaxTiles())
{
}

TileMapCollider2d::~TileMapCollider2d()
{
}

void TileMapCollider2d::Start()
{
}

PhysicsEnd

