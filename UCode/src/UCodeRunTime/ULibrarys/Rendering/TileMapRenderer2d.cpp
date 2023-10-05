#include "TileMapRenderer2d.hpp"
RenderingStart
UComponentData TileMapRenderer::type_Data = { "TileMapRenderer",nullptr};
TileMapRenderer::TileMapRenderer(Entity* entity, IndexType MapX, IndexType MapY) : Renderer2d(entity,&type_Data)
, shader(Shader::Default_Shader(entity->GetGameRunTime()->Get_Library_Edit()))
, DrawOrder(RenderRunTime2d::DrawOrderType_Min)
, _TileMapSizeX(MapX),_TileMapSizeY(MapY), TilesData(Get_MaxTiles())
{
	
}
TileMapRenderer::~TileMapRenderer()
{
	
}

void TileMapRenderer::Start()
{
}
void TileMapRenderer::OnDraw()
{
	auto Entity = GetMyEntity();
	auto Render = GetRenderRunTime();

	Vec2 offset = (Vec2)Vec2i(_TileMapSizeX / 2, _TileMapSizeY / 2);
	Vec2 min = Entity->Get_WorldPosition2D() - offset;
	auto Ro = Entity->Get_WorldRotation2D();
	auto Scale = Entity->Get_WorldScale2D();

	size_t i = 0;


	for (size_t x = 0; x < _TileMapSizeX; x++)
	{
		for (size_t y = 0; y < _TileMapSizeY; y++)
		{
			const auto& Item = TilesData[i];



			Vec2 tilepos = min + Vec2((f32)x, (f32)y);

			RenderRunTime2d::DrawQuad2dData Data = RenderRunTime2d::DrawQuad2dData(tilepos, Scale, Ro);
			Data.Spr = Item.texture;
			Data.color = color * Item.color;
			Data.drawLayer = DrawLayer;
			Data.draworder = DrawOrder;
#if UCodeGameEngineDEBUG
			Data.madeby = this;
#endif
			Render->DrawQuad2d(Data);

			i++;
		}
	}
}
Bounds2d TileMapRenderer::Get_Bounds() const
{
	Bounds2d r;
	r.center = GetMyEntity()->Get_WorldPosition2D();

	const Vec2 tilemapsize = GetMyEntity()->Get_WorldScale2D();
	r.extents =
	{
		tilemapsize.X * (f32)Get_TileMapSizeX() ,
		tilemapsize.Y * (f32)Get_TileMapSizeY()
	};
	return r;
}




RenderingEnd