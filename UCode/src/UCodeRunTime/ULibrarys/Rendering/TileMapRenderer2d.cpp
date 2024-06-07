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
UComponentData TileMapRenderer::type_Data = { "TileMapRenderer",[](Entity* e) {return (Compoent*)new TileMapRenderer(e); },rttr::type::get<UCode::TileMapRenderer>() };
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
	auto Render = GetRenderRunTime();
	auto cam = Render->CurrentCam();
	auto bounds = cam->GetCam_Bounds();


	auto Entity = NativeEntity();
	auto Scale = Entity->WorldScale2D();
	Vec2 min = Entity->WorldPosition2D();
		
	float w = Scale.X;
	float h = Scale.Y;

	auto AssetManager = AssetManager::Get(Render->GetGameLib());

	for (auto& Item : _Tiles)
	{
		Vec2 tilepos = GetTilePos( min + Vec2((f32)Item.xoffset, (f32)Item.yoffset));

		tilepos.X += w / 2;
		tilepos.Y += h / 2;


		RenderRunTime2d::DrawQuad2dData Data = RenderRunTime2d::DrawQuad2dData(tilepos,Scale, {});

		Sprite* sprptr = nullptr;
		{
			NullablePtr<SpritePtr> ptr;
			if (!Item.tile.Has_Asset() && Item.tile.Has_UID())
			{
				auto assetop = AssetManager->FindOrLoad_t<TileAsset>(Item.tile.Get_UID());
				if (assetop.has_value())
				{
					Item.tile = assetop.value()->GetManaged();
				}
			}
			if (Item.tile.Has_Asset())
			{
				auto asset = Item.tile.Get_Asset();
				ptr = &asset->Sprite;
			}

			if (ptr.has_value()) 
			{
				AssetRendering::UpdatePtr(AssetManager,*ptr.value());

				auto& val = *ptr.value();
				if (val.Has_Asset())
				{
					sprptr = val.Get_Asset();
				}
			}
		}
		Data.Spr = sprptr;
		Data.color = color * Item.color;
		Data.drawLayer = DrawLayer;
		Data.draworder = DrawOrder;
#if UCodeGameEngineDEBUG
		Data.madeby = this;
#endif
		Render->DrawQuad2d(Data);


	}

	if (Render->IsDrawingInSceneEditor()) 
	{
		if (Showgrid)
		{
			DrawGrid();
		}
	}
}
UComponentsID TileMapRenderer::Get_TypeID()
{
	return type_Data._Type;
}
void TileMapRenderer::DrawGrid()
{
	auto Render = GetRenderRunTime();
	auto cam = Render->CurrentCam();
	auto bounds = cam->GetCam_Bounds();

	size_t Griddwidth = 40;
	size_t Gridheight = 40;

	Color gridcolor = { 0.7,0.7,0.7 };

	auto pos = NativeEntity()->WorldPosition2D();

	Vec2 start = { pos.X - (Griddwidth / 2),pos.Y - (Gridheight / 2) };
	for (size_t x = 0; x < Griddwidth; x++)
	{
		RenderRunTime2d::Draw2DLineData linedata;
		linedata.color = gridcolor;

		linedata.Start = start;
		linedata.End = start;

		linedata.Start += { (float) x,(float)0 };
		linedata.End += { (float) x,(float)Gridheight };
		linedata.draworder = 0;
		linedata.drawLayer = 0;

		Render->DrawLine2d(linedata);
	}

	for (size_t y = 0; y < Gridheight; y++)
	{
		RenderRunTime2d::Draw2DLineData linedata;
		linedata.color = gridcolor;

		linedata.Start = start;
		linedata.End = start;

		linedata.Start += { (float)0,(float)y };
		linedata.End += { (float)Griddwidth,(float)y };
		linedata.draworder = 0;
		linedata.drawLayer = 0;

		Render->DrawLine2d(linedata);
	}
}
NullablePtr<TileMapRenderer::Tile> TileMapRenderer::Get_Tile(int x, int y)
{
	for (auto& Item : _Tiles)
	{
		if (Item.xoffset == x && Item.yoffset == y)
		{
			return &Item;
		}
	}
	return {};
}
void TileMapRenderer::AddTile(Tile& tile)
{
	AddTileRaw(tile);
}
void TileMapRenderer::AddTileRaw(Tile& tile)
{
	auto v = Get_Tile(tile.xoffset, tile.yoffset);

	if (v.has_value())
	{
		*v.value() = tile;	
	}

	_Tiles.push_back(std::move(tile));
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
Vec2 TileMapRenderer::GetTilePos(Vec2 pos)
{
	Vec2 r;
	r.X = std::floor(pos.X);
	r.Y = std::floor(pos.Y);
	return r;
}

RenderingEnd


