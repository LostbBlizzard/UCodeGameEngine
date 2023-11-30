#pragma once

#include <UCodeRunTime/Rendering/Render.hpp>
#include <UCodeRunTime/Core/GameRunTime.hpp>
#include "UCodeRunTime/CoreSystems/GameFiles.hpp"
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "AppData.hpp"

CoreStart
class UApp
{
public:
	UApp();
	void Init(RenderAPI::WindowData windowdata = RenderAPI::WindowData(800, 540, "UAPP"), const GameFilesData& Data = {});
	void Run();

	inline auto Get_RunTime() {return  _run.get();}
	inline auto Get_Render() { return  _render.get(); }
private:
	void End();
	Unique_ptr<GameRunTime> _run;
	Unique_ptr<RenderAPI::Render> _render;
};
class USeverApp
{
public:
	USeverApp();
	void Init();
	void Run();

	inline auto Get_RunTime() { return  _run.get(); }
private:
	void End();
	Unique_ptr<GameRunTime> _run;
};

CoreEnd
