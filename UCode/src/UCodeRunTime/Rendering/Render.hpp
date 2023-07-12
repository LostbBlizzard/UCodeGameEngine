
#pragma once
#include "RenderAPI.hpp"
#include "WindowData.hpp"
RenderAPIStart



class Render
{
public:
	Render();
	~Render();
	void PreInit();
	void Init(WindowData windowdata,GameRunTime* gameruntime);
	void UpdateFrame();
	
	void Draw(RenderRunTime2d::DrawData& Data,Camera2d* cam);

	void EndRender();
	inline auto Get_RenderAPI() { return  _Render; }
private:
	void ImGuiNewFrame();
	void ImGuiEndFrame();
	RenderAPI* _Render;
	GameRunTime* _Runtime;
	WindowData _WindowData;
	GLFWwindow* Imgui_Context;
};

RenderAPIEnd