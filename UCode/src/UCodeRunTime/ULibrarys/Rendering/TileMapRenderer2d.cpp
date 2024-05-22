#include "TileMapRenderer2d.hpp"

#include "../Serialization/Yaml_Implementation/AsssetPtr.hpp"
#include "../Serialization/Bit_Implementation/AsssetPtr.hpp"

RTTR_REGISTRATION
{
 rttr::registration::class_<UCode::TileMapRenderer>("TileMapRenderer")
 .property("Shader",&UCode::TileMapRenderer::shader)
 .property("Color",&UCode::TileMapRenderer::color)
 .property("DrawLayer",&UCode::TileMapRenderer::DrawLayer)
 .property("DrawOrder",&UCode::TileMapRenderer::DrawOrder);
}

RenderingStart
UComponentData TileMapRenderer::type_Data = { "TileMapRenderer",nullptr};
TileMapRenderer::TileMapRenderer(Entity* entity) : Renderer2d(entity,&type_Data)
, shader(Shader::Default_Shader(entity->NativeGameRunTime()->Get_Library_Edit()))
, DrawOrder(RenderRunTime2d::DrawOrderType_Min)
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
	/*
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
	*/
}
UComponentsID TileMapRenderer::Get_TypeID()
{
	return type_Data._Type;
}
NullablePtr<TileMapRenderer::Tile> TileMapRenderer::Get_Tile(int x, int y)
{
	return {};
}
void TileMapRenderer::AddTile(Tile& tile)
{
}
Bounds2d TileMapRenderer::Get_Bounds() const
{
	return {};
}

void TileMapRenderer::Serialize(USerializer& Serializer) const
{
	Serializer.Write("Shader", shader);
	Serializer.Write("Color", color);
	Serializer.Write("DrawLayer", DrawLayer);
	Serializer.Write("DrawOrder", DrawOrder);
}

void TileMapRenderer::Deserialize(UDeserializer& Serializer)
{
	Serializer.ReadType("Shader", shader);
	Serializer.ReadType("Color", color);
	Serializer.ReadType("DrawLayer", DrawLayer, DrawLayer);
	Serializer.ReadType("DrawOrder", DrawOrder, DrawOrder);
}


void TileData::PushData(USerializer& node) const
{
	node.Write("_Sprite", Sprite);
	node.Write("_Color", Color);
}

bool TileData::FromString(TileData& out, UDeserializer& text)
{
	text.ReadType("_Sprite",out.Sprite);
	text.ReadType("_Color", out.Color);
	return true;
}

bool TileData::FromFile(TileData& out, const Path& Path)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V);
	if (A)
	{
		return FromString(out, V);
	}
	return A;
}

bool TileData::ToFile(const Path& path, const SpriteData& data, USerializerType Type)
{
	USerializer V(Type);
	data.PushData(V);
	return V.ToFile(path);
}

RenderingEnd


