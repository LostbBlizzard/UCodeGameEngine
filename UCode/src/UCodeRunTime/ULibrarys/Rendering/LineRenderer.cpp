#include "LineRenderer.hpp"
#include "../Serialization/Yaml_Implementation/AsssetPtr.hpp"
#include "../Serialization/Bit_Implementation/AsssetPtr.hpp"

RTTR_REGISTRATION
{
 rttr::registration::class_<UCode::LineRenderer>("LineRender")
 .property("Shader",&UCode::LineRenderer::shader)
 .property("Color",&UCode::LineRenderer::color)
 .property("DrawLayer",&UCode::LineRenderer::DrawLayer)
 .property("DrawOrder",&UCode::LineRenderer::DrawOrder);
}

RenderingStart

void LineRenderer::Start()
{
}

void LineRenderer::OnDraw()
{
	auto Render = GetRenderRunTime();
	auto e = this->NativeEntity();

	RenderRunTime2d::Draw2DLineData data;
	data.color = this->color;
	data.Thickness = this->LineThickness;
	data.Start = e->LocalPosition2D() + this->_Start;
	data.End = e->LocalPosition2D() + this->_End;
	Render->DrawLine2d(data);
}

UComponentsID LineRenderer::Get_TypeID()
{
	return "LineRender";
}
UComponentData LineRenderer::type_Data = { Get_TypeID(),[](Entity* E) {return (Compoent*)new LineRenderer(E);},rttr::type::get<UCode::LineRenderer>()};
LineRenderer::LineRenderer(Entity* entity) :Renderer2d(entity,&type_Data)
{

}

LineRenderer::~LineRenderer()
{
}

void LineRenderer::Serialize(USerializer& Serializer) const
{
	Serializer.Write("Shader", shader);
	Serializer.Write("Color", color);
	Serializer.Write("DrawLayer", DrawLayer);
	Serializer.Write("DrawOrder", DrawOrder);
	Serializer.Write("Start", _Start);
	Serializer.Write("End", _End);
}

void LineRenderer::Deserialize(UDeserializer& Serializer)
{
	Serializer.ReadType("Shader", shader);
	Serializer.ReadType("Color", color);
	Serializer.ReadType("DrawLayer", DrawLayer, DrawLayer);
	Serializer.ReadType("DrawOrder", DrawOrder, DrawOrder);
	Serializer.ReadType("Start", _Start, _Start);
	Serializer.ReadType("End", _End, _End);
}

Bounds2d LineRenderer::Get_Bounds() const
{
	return Bounds2d();
}
RenderingEnd

