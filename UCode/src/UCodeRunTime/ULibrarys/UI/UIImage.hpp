#pragma once
#include "UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp"
#include "UIRunTime.hpp"

#include "UINamespace.hpp"
UIStart
class UIImage  final :public Renderer2d
{
private:
	void Start() override;
	void OnDraw()override;
public:
	UIImage(Entity* e);
	~UIImage();


	static UComponentsID Get_TypeID();
	static UComponentData type_Data;
public:
	UIInfo UIData;

	Shader* shader;
	Sprite* texture;
	Color color;
	bool flipX;
	bool flipY;
	RenderRunTime2d::DrawLayer_t DrawLayer;
	RenderRunTime2d::DrawOrder_t DrawOrder;
};
UIEnd

