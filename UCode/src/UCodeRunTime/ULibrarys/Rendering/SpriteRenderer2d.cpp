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
shader(Shader::Default_Shader(entity->GetGameRunTime()->Get_Library_Edit())),
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
	Serializer.Write("Color", color);
	Serializer.Write("flipX", flipX);
	Serializer.Write("flipY", flipY);
	Serializer.Write("DrawLayer", DrawLayer);
	Serializer.Write("DrawOrder", DrawOrder);
}
void SpriteRenderer::Deserialize(UDeserializer& Serializer)
{

	Serializer.ReadType("Shader", shader, shader);
	Serializer.ReadType("Sprite", sprite, sprite);
	Serializer.ReadType("Color", color, color);

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
	auto Entity = GetMyEntity();
	auto Render = GetRenderRunTime();
	auto Scale =Entity->Get_WorldScale2D();
	if (flipX) {Scale.X = -Scale.X;}
	if (flipY) { Scale.Y = -Scale.Y; }

	auto AssetManager = AssetManager::Get(Render->GetGameLib());

	AssetRendering::UpdatePtr(AssetManager,sprite);

	Vec2 pos = Entity->Get_WorldPosition2D();
	Vec2 size = Scale;
	Vec2 Rotation = Entity->Get_WorldRotation2D();
	
	AssetRendering::DrawQuad2dData Data(pos - Vec2(size.X/2, size.Y/2), size, Rotation);
	

	Data.Spr = sprite;
	Data.color = color;
	Data.drawLayer = DrawLayer;
	Data.draworder = DrawOrder;
#ifdef DEBUG
	Data.madeby = this;
#endif

	AssetRendering::DrawQuad2d(Render, Data);
}
Bounds2d SpriteRenderer::Get_Bounds() const
{
	auto Entity = GetMyEntity();
	Bounds2d r;
	r.center = Entity->Get_WorldPosition2D();
	const Vec2 size = Entity->Get_WorldScale2D();
	r.extents = size - Vec2(size.X / 2, size.Y / 2);
	return r;
}
RenderingEnd
