#pragma once
#include "RenderRunTime2d.hpp"


#include <UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp>
RenderingStart
class CoreModule;
class SpriteRenderer final : public Renderer2d
{
	friend CoreModule;
private:
	void Start() override;
	void OnDraw() override;
public:
	SpriteRenderer(Entity* entity);
	~SpriteRenderer() override;

	void Serialize(USerializer& Serializer)  const override;

	void Deserialize(UDeserializer& Serializer) override;
	

	static UComponentsID Get_TypeID();
public:
	Bounds2d Get_Bounds() const;

	ShaderPtr shader;
	SpritePtr sprite;
	Color color;
	bool flipX;
	bool flipY;
	RenderRunTime2d::DrawLayer_t DrawLayer;
	RenderRunTime2d::DrawOrder_t DrawOrder;
	static UComponentData type_Data;
};
RenderingEnd
