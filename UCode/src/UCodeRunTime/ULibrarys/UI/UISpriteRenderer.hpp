#pragma once


#include "UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp"
#include "UIRunTime.hpp"

#include "UINamespace.hpp"
UIStart
class UISpriteRenderer  final :public Renderer2d
{
private:
	void Start() override;
	void OnDraw()override;
public:
	UISpriteRenderer(Entity* e);
	~UISpriteRenderer();



public:
	UIInfo UIData;

	Shader* shader;
	Sprite* texture;
	Color color;
	bool flipX;
	bool flipY;
	RenderRunTime2d::DrawLayer_t DrawLayer;
	RenderRunTime2d::DrawOrder_t DrawOrder;
	static UComponentData type_Data;
};
UIEnd

