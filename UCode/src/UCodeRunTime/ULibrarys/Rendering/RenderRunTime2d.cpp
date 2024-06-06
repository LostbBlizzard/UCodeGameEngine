#include "RenderRunTime2d.hpp"

RenderingStart
Renderer2d::Renderer2d(Entity* entity, UComponentData* typeData) : Compoent(entity, typeData)
{
	GetRenderRunTime()->All_Renderers.push_back(this);
}
Renderer2d::~Renderer2d()
{
	if (Get_Scene()->Get_IsDestroyed()) { return; }
	auto& All_Renders = GetRenderRunTime()->All_Renderers;
	for (auto it = All_Renders.begin(); it != All_Renders.end();)
	{

		if (*it == this)
		{
			All_Renders.erase(it);
			return;
		}
		++it;
	}
}

RenderRunTime2d* Renderer2d::GetRenderRunTime()
{
	return RenderRunTime2d::GetRenderRunTime(GetGameRunTime());
}

UComponentData RenderRunTime2d::type_Data = { "RenderRunTime2d",nullptr};
RenderRunTime2d::RenderRunTime2d(Entity* entity) : Compoent(entity,&type_Data)
{
	
}
RenderRunTime2d::~RenderRunTime2d()
{
	GetGameRunTime()->ReMoveStaticComponent((size_t)StaticRuntimeComponentsIndex::RenderRunTime);
	Entity::Destroy(NativeEntity());
}
RenderRunTime2d* RenderRunTime2d::FindRenderRunTime(const GameRunTime* e)
{
	const auto index = (size_t)StaticRuntimeComponentsIndex::RenderRunTime;
	auto item = e->Get_StaticComponent(index);
	return (RenderRunTime2d*)item;
}
RenderRunTime2d* RenderRunTime2d::GetRenderRunTime(GameRunTime* e)
{
	const auto index = (size_t)StaticRuntimeComponentsIndex::RenderRunTime;
	auto item = e->Get_StaticComponent(index);
	if (item == nullptr)
	{
		Entity* newe = e->NewEntityOnRunTimeScene();
		auto r = new RenderRunTime2d(newe);
		newe->MoveCompoent(r);
		e->SetStaticComponent(index, r);
		return r;
	}
	return (RenderRunTime2d*)item;
}
void RenderRunTime2d::UpdateDrawData()
{
	ReSetStats();

	//resetdata
	_DrawData.Quad2d.clear();
	_DrawData.Lines2d.clear();
	
	for (size_t i = 0; i < All_Renderers.size(); i++)
	{
		auto Item = All_Renderers[i];
		if (Item->Get_IsDestroyed() || Item->NativeEntity()->Get_IsDestroyed()) 
		{
			continue;
		}
		if (Item->Get_IsActive_InRunTime())
		{
			Item->OnDraw();
		}
	}
}
void RenderRunTime2d::ReSetStats()
{
	DataStats.DrawCount = 0;
	DataStats.QuadCount = 0;
}

bool RenderRunTime2d::DrawData::HasAnyPlaceHolders() const
{
	for (auto& Item : Quad2d)
	{
		if (Item.Spr)
		{
			if (Item.Spr->Get_texture()->IsPlaceHolder())
			{
				return true;
			}
		}
	}
	return false;
}
RenderingEnd


