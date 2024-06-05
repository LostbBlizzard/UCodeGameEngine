#pragma once
#include "RenderRunTime2d.hpp"


#include <UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp>
RenderingStart

class LineRenderer final : public Renderer2d
{
	friend CoreModule;
private:
	void Start() override;
	void OnDraw() override;
public:
	LineRenderer(Entity* entity);
	~LineRenderer() override;

	void Serialize(USerializer& Serializer)  const override;

	void Deserialize(UDeserializer& Serializer) override;

	rttr::instance Get_Rttr_Instance() override { return rttr::instance(this); }

	static UComponentsID Get_TypeID();
public:
	Bounds2d Get_Bounds() const;

	ShaderPtr shader;
	Color color;
	RenderRunTime2d::DrawLayer_t DrawLayer = {};
	RenderRunTime2d::DrawOrder_t DrawOrder = {};
	Vec2 _Start;
	Vec2 _End = { 1,0 };
	float LineThickness = 0.2;
	static UComponentData type_Data;
};

RenderingEnd
