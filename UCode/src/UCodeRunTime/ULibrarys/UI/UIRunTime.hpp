#pragma once



#include <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include <UCodeRunTime/ULibrarys/Rendering/Camera2d.hpp>

#include "UINamespace.hpp"
UIStart


struct NormVec2
{
	Vec2 Value;
	NormVec2() :Value(){}
	NormVec2(Vec2 v) :Value(v) {}
	NormVec2(f32 x, f32 y) : Value(x,y){}
};

struct UIInfo
{
	NormVec2 Pos;
	NormVec2 Size;
	static NormVec2 Get_InfoSize(Vec2i Pixsize, Vec2i windowsizeRef = {1920 ,1080})
	{
		return { 0,0 };
	}
};

class UIHelper
{
public:
	static const RenderRunTime2d::DrawLayer_t UI_DrawLayer = 0xff;

	static NormVec2 ScreenPosToNorm(const Vec2 ScreenPos, const Camera2d* Cam);
};

class UIRunTIme  final :private Compoent
{
private:
	UIRunTIme(Entity* e);
	~UIRunTIme();
	void Start() override;
public:
	typedef Delegate<void> ClickCallBackType;

	
	
	
	

	void AddClickCallBack()
	{

	}

	static UIRunTIme* GetUI(GameRunTime* e);
	static UIRunTIme* FindUI(const GameRunTime* e);
private:
	static UComponentData type_Data;
};

UIEnd

