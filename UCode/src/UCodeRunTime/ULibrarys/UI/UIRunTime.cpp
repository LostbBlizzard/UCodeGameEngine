#include "UIRunTime.hpp"


UIStart

NormVec2 UIHelper::ScreenPosToNorm(const Vec2 ScreenPos, const Camera2d* Cam)
{
	/*
	auto winsize = Cam->Get_windowSize();
	Vec2 r = { ScreenPos.X / winsize.X, ScreenPos.Y / winsize.Y };
*/
	return{};
}

UComponentData UIRunTIme::type_Data = { "UIRunTIme",nullptr};
UIRunTIme::UIRunTIme(Entity* e) : Compoent(e,&type_Data)
{
	
}

UIRunTIme::~UIRunTIme()
{
	GetGameRunTime()->ReMoveStaticComponent((size_t)StaticRuntimeComponentsIndex::UIRunTIme);
	Entity::Destroy(NativeEntity());
}

void UIRunTIme::Start()
{
}

UIRunTIme* UIRunTIme::GetUI(GameRunTime* e)
{
	const auto index = (size_t)StaticRuntimeComponentsIndex::UIRunTIme;
	auto item = e->Get_StaticComponent(index);
	if (item == nullptr)
	{
		Entity* newe = e->NewEntityOnRunTimeScene();
		auto r = new UIRunTIme(newe);
		newe->MoveCompoent(r);
		e->SetStaticComponent(index, r);
		return r;
	}
	return (UIRunTIme*)item;
}

UIRunTIme* UIRunTIme::FindUI(const GameRunTime* e)
{
	const auto index = (size_t)StaticRuntimeComponentsIndex::UIRunTIme;
	auto item = e->Get_StaticComponent(index);
	return (UIRunTIme*)item;
}

UIEnd

