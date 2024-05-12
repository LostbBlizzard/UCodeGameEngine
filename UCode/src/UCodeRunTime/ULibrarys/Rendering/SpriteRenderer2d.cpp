#include "SpriteRenderer2d.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"

#include "../Serialization/Yaml_Implementation/AsssetPtr.hpp"
#include "../Serialization/Bit_Implementation/AsssetPtr.hpp"
RenderingStart


UComponentsID SpriteRenderer::Get_TypeID()
{
	return "SpriteRenderer";
}
UComponentData SpriteRenderer::type_Data = { Get_TypeID(),[](Entity* E) {return (Compoent*)new SpriteRenderer(E); }};
SpriteRenderer::SpriteRenderer(Entity* entity) : Renderer2d(entity,&type_Data),
shader(Shader::Default_Shader(entity->NativeGameRunTime()->Get_Library_Edit())),
sprite(nullptr),
flipX(false),
flipY(false),
color(Color::White()),
DrawLayer(RenderRunTime2d::DrawOrderType_Min),
DrawOrder(RenderRunTime2d::DrawOrderType_Min)
{

}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Serialize(USerializer& Serializer) const
{
	Serializer.Write("Shader", shader);
	Serializer.Write("Sprite", sprite);
	Serializer.Write("Color2", color);
	
	Serializer.Write("flipX", flipX);
	Serializer.Write("flipY", flipY);

	Serializer.Write("DrawLayer", DrawLayer);
	Serializer.Write("DrawOrder", DrawOrder);
}
void SpriteRenderer::Deserialize(UDeserializer& Serializer)
{
	Serializer.ReadType("Shader", shader, shader);
	Serializer.ReadType("Sprite", sprite, sprite);
	Serializer.ReadType("Color2", color,color);

	Serializer.ReadType("flipX", flipX, flipX);
	Serializer.ReadType("flipX", flipY, flipY);

	Serializer.ReadType("DrawLayer", DrawLayer, DrawLayer);
	Serializer.ReadType("DrawOrder", DrawOrder, DrawOrder);

}

void SpriteRenderer::Start()
{
}

void SpriteRenderer::OnDraw()
{
	auto Entity = NativeEntity();
	auto Render = GetRenderRunTime();
	auto Scale =Entity->WorldScale2D();
	if (flipX) {Scale.X = -Scale.X;}
	if (!flipY) { Scale.Y = -Scale.Y; }

	auto AssetManager = AssetManager::Get(Render->GetGameLib());

	AssetRendering::UpdatePtr(AssetManager,sprite);

	
	
	Vec2 pos = Entity->WorldPosition2D();
	Vec2 size;

	if (sprite.Has_Asset())
	{
		auto sp = sprite.Get_Asset();
		auto ppu = sp->Get_texture()->PixelsPerUnit;

		size = {(f32)sp->Get_Width(),(f32)sp->Get_Height()};
		size *= (f32)1/(f32)ppu;

		size.X *= Scale.X;	
		size.Y *= Scale.Y;	
	}
	else
	{
		size = Scale;
	}

	Vec2 Rotation = Entity->WorldRotation2D();
	
	//AssetRendering::DrawQuad2dData Data(pos - Vec2(size.X/2, size.Y/2), size, Rotation);
	AssetRendering::DrawQuad2dData Data(pos, size, Rotation);

	Data.Spr = sprite;
	Data.color = color;
	Data.drawLayer = DrawLayer;
	Data.draworder = DrawOrder;
#if UCodeGEDebug
	Data.madeby = this;
#endif

	AssetRendering::DrawQuad2d(Render, Data);
}
Bounds2d SpriteRenderer::Get_Bounds() const
{
	auto Entity = NativeEntity();
	Bounds2d r;
	r.center = Entity->WorldPosition2D();
	const Vec2 size = Entity->WorldScale2D();
	r.extents = size - Vec2(size.X / 2, size.Y / 2);
	return r;
}
RenderingEnd
