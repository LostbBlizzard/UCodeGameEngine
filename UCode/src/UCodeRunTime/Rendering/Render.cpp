#include "Render.hpp"
#include  <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include <Imgui/imgui.h>
#include <Imgui/backends/imgui_impl_glfw.h>
#include <Imgui/backends/imgui_impl_opengl3.h>

RenderAPIStart


Render::Render()
{
	_Render = nullptr;
	_Runtime = nullptr;
	Imgui_Context = nullptr;
}

Render::~Render()
{
}

void Render::PreInit()
{
	RenderAPI::RenderSetUp();
}

void Render::Init(WindowData windowdata,GameRunTime* gameruntime)
{
	_Runtime = gameruntime;
	_Render =new RenderAPI(gameruntime);
	_Render->Init(windowdata);
	_WindowData = windowdata;
	//SetCallBacks

	if (windowdata._filedroped)
	{

	}
}

void Render::ImGuiNewFrame()
{
	Imgui_Context = glfwGetCurrentContext();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void Render::ImGuiEndFrame()
{
	glfwMakeContextCurrent(Imgui_Context);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	ImGuiIO& io = ImGui::GetIO();
	
	// Update and Render additional Platform Windows
	   // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	   //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
	//ImGui
}

void Render::UpdateFrame()
{
	auto Old = glfwGetCurrentContext();
	auto Win = _Render->Get_Window();
	if (Win && Win != Old)
	{
		glfwMakeContextCurrent(Win);
	}

	if (_Render->IsRuning)
	{
		auto RenderRunTime = RenderRunTime2d::FindRenderRunTime(_Runtime);
		
		if (RenderRunTime)
		{
			Camera2d* MainCam = Camera2d::FindMainCam(_Runtime);
			bool ImguDraw = _WindowData.ImGui_Init;


			if (ImguDraw) { ImGuiNewFrame(); }
			
			if (MainCam)
			{
				RenderRunTime->UpdateDrawData(MainCam);
			}


			_Render->UpdateCamWindowSize();
			if (MainCam)
			{
				FrameBuffer& buff = MainCam->Get_Buffer();
				if (_Render->HasWindow()) {
					MainCam->Get_Buffer().Set_SwapChinTarget(true);
				}

				_Render->_DrawOpenGl(RenderRunTime->Get_DrawData(), MainCam);


				buff.UnBind();
			}
			if (ImguDraw) { ImGuiEndFrame(); }

			_Render->UpdateScreen();


			if (MainCam)
			{
				FrameBuffer& buff = MainCam->Get_Buffer();
				//buff.UnBind();
			}
			_Render->ProcessInput();
		}
		else
		{
			_Render->UpdateScreen();
			_Render->ProcessInput();
		}
	}
	else
	{
		EndRender();
	}

	glfwMakeContextCurrent(Old);
}

void Render::Draw(RenderRunTime2d::DrawData& Data, Camera2d* cam)
{
	auto Old = glfwGetCurrentContext();
	auto Win = _Render->Get_Window();
	if (Win && Win != Old)
	{
		glfwMakeContextCurrent(Win);
	}

	auto RenderRunTime = RenderRunTime2d::FindRenderRunTime(_Runtime);
	

	if (cam)
	{
		FrameBuffer& buff = cam->Get_Buffer();
		if (_Render->HasWindow()) {
			cam->Get_Buffer().Set_SwapChinTarget(true);
		}

		_Render->_DrawOpenGl(RenderRunTime->Get_DrawData(), cam);


		buff.Bind();
	}

	if (cam)
	{
		FrameBuffer& buff = cam->Get_Buffer();
		buff.UnBind();
	}

	glfwMakeContextCurrent(Old);
}


void Render::EndRender()
{
	auto Old = glfwGetCurrentContext();
	auto Win = _Render->Get_Window();
	if (Win && Win != Old)
	{
		glfwMakeContextCurrent(Win);
	}

	delete _Render;
	_Render = nullptr;

	if (Win && Win != Old)
	{
		glfwMakeContextCurrent(Old);
	}
}
RenderAPIEnd