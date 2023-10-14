#include "OpenGlRender.hpp"
#include "OpenGLInput.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <algorithm>

#include <Imgui/imgui.h>
#include <Imgui/backends/imgui_impl_glfw.h>
#include <Imgui/backends/imgui_impl_opengl3.h>
#include <Imgui/imgui_internal.h>
//Ucode
#include  <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include  <UCodeRunTime/ULibrarys/InputManger/InputManger.hpp>
#include  <UCodeRunTime/BasicTypes.hpp>
#include  <UCodeRunTime/ULibrarys/Math_Library.hpp>
RenderAPIStart
OpenGlRender::OpenGlRender(GameRunTime* run)
{
    RunTime = run;
    _windowdata = {};
	window = nullptr;
    IsRuning = false;
    oldwindowWidth = -1;
    oldwindowHeight = -1;


    Classes.push_back(this);
    
    

    IndexCount = 0;
    NextTextureSlot = 0;
    WhiteTexture = nullptr;
    WhiteTextureSlot = 0;

    QuadVA = nullptr;
    QuadVB = nullptr;
    QuadIB = nullptr;

    QuadBuffer = nullptr;
    QuadBufferPtr = nullptr;
    
}

OpenGlRender::~OpenGlRender()
{
    for (auto it = Classes.begin(); it != Classes.end(); ++it)
    {
        if (*it == this)
        {
            Classes.erase(it);
            return;
        }
    }
   
}
void OpenGlRender::RenderSetUp()
{
    glfwSetErrorCallback(glfwerror_callback);


    if (glfwInit() == GL_FALSE)
    {
        throw std::runtime_error("glfwInit failed");
    }
  
}

void OpenGlRender::Init(WindowData windowdata)
{
    _windowdata = windowdata;

    if (_windowdata.GenNewWindow)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//IMgui Update them
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_VISIBLE, true);
        glfwWindowHint(GLFW_FOCUSED, true);
        glfwWindowHint(GLFW_DECORATED, true);

        glfwWindowHint(GLFW_FLOATING, false);

        window = glfwCreateWindow(_windowdata.width, _windowdata.height, _windowdata.WindowName.c_str()
            ,nullptr,  
            (GLFWwindow*)_windowdata.shared_window);



        oldwindowWidth = -1;
        oldwindowHeight = -1;
        if (!window)
        {
            glfwTerminate();
        }

        glfwMakeContextCurrent(window);


        if (_windowdata.ImGui_Init)
        {


            if (glewInit() != GLEW_OK)
            {
                throw std::runtime_error("glewInit failed");
            }
            //ImGui
            ImGui::CreateContext();


            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            //io.IniFilename = nullptr;
            
            //OpenGlRender::SetStyle_WoodLandDay();
            SetStyle_Gray();
            //OpenGlRender::SetStyle_Dark();

            // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
            ImGuiStyle& style = ImGui::GetStyle();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                style.WindowRounding = 0.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            }

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            const char* glsl_version = "#version 130";
            ImGui_ImplOpenGL3_Init(glsl_version);

            //ImGui 
        }


        // 
        GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GlCall(glEnable(GL_BLEND));


        if (windowdata.SetUpInputs) { Input::Set_InputCallBacks(window); }
    }

    UpdateCamWindowSize();
    IsRuning = true;
    //
   
    Init();

}
void OpenGlRender::SetWindowIcon(const Texture& tex)
{
    if (window)
    {
        GLFWimage icon;
        icon.height = tex.Get_Height();
        icon.width = tex.Get_Width();
        icon.pixels = (unsigned char*)tex.Get_ColorData();

        glfwSetWindowIcon(window, 1, &icon);
    }
}

void OpenGlRender::_DrawOpenGl(RenderRunTime2d::DrawData& Data, Camera2d* cam)
{
    auto Width = cam->Get_WindowWidth();
    auto Height = cam->Get_WindowHeight();
    if (Width <= 0 && Height <= 0) { return; }

    cam->UpdateCamData();


    auto& Buffer = cam->Get_Buffer();

    if (!Buffer.Is_SwapChinTarget())
    {
        Buffer.Bind();
    }

    GlCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));//Background color
    GlCall(glClear(GL_COLOR_BUFFER_BIT));
    GlCall(glViewport(0, 0, Width, Height));


    glm::mat4 Vp = cam->Get_ViewProjectionMatrix();
    glm::mat4 Tranform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    UseingShader->SetUniformMat4f("_ViewProj", Vp);
    UseingShader->SetUniformMat4f("_Transfrom", Tranform);


    BeginBatch();
    _DrawQuads2d(Data);
    EndBatch();
    Flush();


    Buffer.UnBind();

}
void OpenGlRender::SetStyle_WoodLandDay(ImGuiStyle* dst)
{
    //Hippoptamus gray
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    style->FrameRounding = 4.0f;

    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.20f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.49f, 0.29f, 0.23f, 0.61f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.61f, 0.61f, 0.61f, 0.61f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.07f, 0.03f, 0.03f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.49f, 0.29f, 0.78f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.49f, 0.29f, 0.61f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.02f, 0.40f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.03f, 0.03f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.03f, 0.03f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.12f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.14f, 0.51f, 0.31f, 0.74f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.12f, 0.49f, 0.29f, 0.78f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.38f, 0.18f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.12f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.12f, 0.49f, 0.29f, 0.78f);
    colors[ImGuiCol_Button] = ImVec4(0.40f, 0.20f, 0.13f, 0.61f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.20f, 0.13f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.29f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.38f, 0.18f, 0.92f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.49f, 0.29f, 0.92f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.12f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.49f, 0.29f, 0.23f, 0.61f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.12f, 0.49f, 0.29f, 0.92f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.20f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.09f, 0.47f, 0.27f, 0.98f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.12f, 0.49f, 0.29f, 0.92f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.03f, 0.41f, 0.21f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.25f, 0.17f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.12f, 0.49f, 0.29f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.07f, 0.03f, 0.03f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.09f, 0.07f, 0.07f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.36f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.27f, 0.20f, 1.00f);


}
void OpenGlRender::SetStyle_WoodLandNight(ImGuiStyle* dst)
{

}
void OpenGlRender::SetStyle_Gray(ImGuiStyle* dst)
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.89f, 0.42f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.89f, 0.42f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.89f, 0.42f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.67f, 0.67f, 0.67f, 0.39f);
    colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.41f, 0.42f, 0.44f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.89f, 0.42f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.29f, 0.30f, 0.31f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.89f, 0.42f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.09f, 0.83f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.34f, 0.36f, 0.83f);
    colors[ImGuiCol_TabActive] = ImVec4(0.23f, 0.23f, 0.24f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);





}
void OpenGlRender::SetStyle_Dark(ImGuiStyle* dst)
{
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.18f, 0.15f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.02f, 0.04f, 0.02f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.22f, 0.16f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.22f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.05f, 0.66f, 0.09f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.03f, 0.82f, 0.47f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.21f, 1.00f, 0.56f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
}
OpenGlRender* OpenGlRender::GetOpenGlRender(const GLFWwindow* window)
{
    for (size_t i = 0; i < Classes.size(); i++)
    {
        OpenGlRender* Item = Classes[i];
        if (Item->window == window)
        {
            return Item;
        }
    }
    return nullptr;
}
void OpenGlRender::UpdateCamWindowSize()
{
    i32 windowWidth, windowHeight;

    RenderRunTime2d* rtime = RenderRunTime2d::FindRenderRunTime(RunTime);
    if (rtime == nullptr || window == nullptr) { return; }

    GlCall(glfwGetWindowSize(window, &windowWidth, &windowHeight));

    bool CanDraw = windowWidth != 0 && windowHeight != 0;
    if (CanDraw)
    {
        if (oldwindowWidth != windowWidth || oldwindowHeight != windowHeight)
        {
            oldwindowWidth = windowWidth;
            oldwindowHeight = windowHeight;


            if (Camera2d* MainCam = Camera2d::FindMainCam(RunTime))
            {
                MainCam->API_Set_WindowSize(windowWidth, windowHeight);
                MainCam->UpdateCamData();
            }
        }

    }
}

void OpenGlRender::UpdateScreen()
{
    if (window)
    {
        GlCall(glfwSwapBuffers(window));
    }
   
}


void OpenGlRender::ProcessInput()
{
   

    if (window)
    { /* Poll for and process events */

        if (_windowdata.UpdateAppIfInputUpdate) 
        {
            GlCall(glfwWaitEvents());
        }
        else
        {
            GlCall(glfwPollEvents());
        }

        if (glfwWindowShouldClose(window))
        {
            RunTime->StopRunTime();
            IsRuning = false;
        }
    }
}

void OpenGlRender::EndRender()
{

    Shutdown();
    if (_windowdata.ImGui_Init) {
        //ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }   //ImGui

    glfwDestroyWindow(window);

    if (_windowdata.ImGui_Init) {
        glfwTerminate();
    }

}

void OpenGlRender::glfwerror_callback(int error, const char* description)
{
    UCODE_ENGINE_ERROR("Glfw ERROR(" << error << "):" << description);
}

static const size_t MaxQuadCount = 1000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;
void OpenGlRender::Init()
{
    i32 Maxtexureslots;
    GlCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &Maxtexureslots));
    TextureSlots.resize(Maxtexureslots);

    for (size_t i = 0; i < TextureSlots.size(); i++)
    {
        TextureSlots[i] = nullptr;
    }

    UseingShader = Shader::Default_Shader(RunTime->Get_Library_Edit());



    const i32 maxSamplers = 32;
    i32 samplers[maxSamplers];
    for (i32 i = 0; i < maxSamplers; i++)
    {
        samplers[i] = i;
    }
    UseingShader->SetUniform1iv("F_Textures", maxSamplers, samplers);


    QuadBuffer = std::make_unique<RenderRunTime2d::Vertex[]>(MaxVertexCount);
    QuadVA = std::make_unique<VertexArray>();
    QuadVB = std::make_unique<VertexBuffer>(nullptr, MaxVertexCount * sizeof(RenderRunTime2d::Vertex));

    VertexBufferLayout layout = VertexBufferLayout();


    layout.Push_float(3);//pos
    layout.Push_float(4);//color
    layout.Push_float(2);//TexCoord
    layout.Push_float(1);//TexIndex


    QuadVA->AddBuffer(QuadVB.get(), layout);


    Unique_array<u32> indices =std::make_unique<u32[]>(MaxIndexCount);


   u32 offset = 0;
    for (auto i = 0; i < MaxIndexCount; i += 6)
    {

        indices[i + 0] = 0 + offset;//0
        indices[i + 1] = 1 + offset;//1
        indices[i + 2] = 2 + offset;//2

        indices[i + 3] = 2 + offset;//2
        indices[i + 4] = 3 + offset;//3
        indices[i + 5] = 0 + offset;//0

        offset += 4;
    }

    QuadIB = std::make_unique<IndexBuffer>(indices.get(), MaxIndexCount);
    

    //white texture
    auto rgb32data = (Color32)Color::White();
    WhiteTexture =std::make_unique<Texture>(1, 1, &rgb32data);
    WhiteTextureSlot = 0;
    TextureSlots[WhiteTextureSlot] = WhiteTexture.get();

    NextTextureSlot = 1;
}
void OpenGlRender::Shutdown()
{
  
}

void OpenGlRender::BeginBatch()
{
    QuadBufferPtr = QuadBuffer.get();
}

void OpenGlRender::EndBatch()
{
    GLsizeiptr size = (GLsizeiptr)QuadBufferPtr - (GLsizeiptr)QuadBuffer.get();
    QuadVB->UpdateData(QuadBuffer.get(), size);
}

void OpenGlRender::Flush()
{

    for (u32 i = 0; i < NextTextureSlot; i++)
    {
        auto& Item = TextureSlots[i];
        Item->TryUploadTexToGPU();
        glBindTextureUnit(i, Item->Get_RendererID());
    }

    QuadVA->Bind();


    GlCall(glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr));


   
    IndexCount = 0;
    NextTextureSlot = 1;//Seting to not WhiteTexture.
}
void OpenGlRender::_DrawQuad2d(RenderRunTime2d::DrawQuad2dData& Data)
{

    if (IndexCount >= MaxIndexCount || NextTextureSlot > GetMaxTextureSlots() - 1)
    {
        EndBatch();
        Flush();

        BeginBatch();
    }

    i32 textureindex = -1;

    const auto pos = Data.pos;
    const auto color = Data.color;
    const auto size = Data.size;

    Texture* tex;
    Vec2 TC[4];
    if (Data.Spr)
    {
        tex = Data.Spr->Get_texture();
        f32 w = (f32)tex->Get_Width();
        f32 h = (f32)tex->Get_Height();

        f32 sx = (f32)Data.Spr->Get_Xoffset();
        f32 sy = (f32)Data.Spr->Get_Yoffset();
        f32 sw;
        f32 sh;

        if (Data.Spr->Get_Width() == Sprite::GetTexureSize) {
            sw = (f32)tex->Get_Width();
        }
        else
        {
            sw = (f32)Data.Spr->Get_Width();
        }

        if (Data.Spr->Get_Height() == Sprite::GetTexureSize)
        {
            sh = (f32)tex->Get_Height();
        }
        else
        {
            sh = (f32)Data.Spr->Get_Height();
        }



        //float A =w/sw


        TC[0] = { sx / w      ,sy / h };
        TC[1] = { (sx + sw) / w, sy / h };
        TC[2] = { (sx + sw) / w,(sy + sh) / h };
        TC[3] = { sx / w      ,(sy + sh) / h };

    }
    else
    {
        tex = nullptr;


        TC[0] = { 0.0f, 0.0f };
        TC[1] = { 1.0f,0.0f };
        TC[2] = { 1.0f,1.0f };
        TC[3] = { 0.0f,1.0f };
    }

    if (tex)
    {
        for (size_t i = 0; i < GetMaxTextureSlots(); i++)
        {
            if (TextureSlots[i] == tex)
            {
                textureindex = (i8)i;
                break;
            }
        }
    }
    else
    {
        textureindex = WhiteTextureSlot;
    }

    if (textureindex == -1)//is not in Texture Slots.
    {
        textureindex = NextTextureSlot;
        TextureSlots[NextTextureSlot] = tex;
        NextTextureSlot++;
    }

    const f32 Zpos = 0;
    Vec3 TryPos[]
    {
        { pos.X         ,pos.Y         , Zpos },
        { pos.X + size.X,pos.Y         , Zpos },
        { pos.X + size.X,pos.Y + size.Y, Zpos },
        { pos.X         ,pos.Y + size.Y, Zpos }
    };
    {//rotation stuff here
        const f32 RotationY = Data.rotation.Y;
        const f32 RotationYRad = RotationY * Math::Deg2Rad;

        Vec2* TryPos2[]
        {
            (Vec2*)&TryPos[0],
            (Vec2*)&TryPos[1],
            (Vec2*)&TryPos[2],
            (Vec2*)&TryPos[3],
        };
        Vec2 Quadat0_0[4]
        {
             {},
             { size.X ,0.0f },
             { size.X ,size.Y },
             { 0.0f   ,size.Y },
        };
        Quadat0_0[0] -= {0.5f, 0.5f};
        Quadat0_0[1] -= {0.5f, 0.5f};
        Quadat0_0[2] -= {0.5f, 0.5f};
        Quadat0_0[3] -= {0.5f, 0.5f};

        *TryPos2[0] = Quadat0_0[0].GetRotated(RotationYRad);
        *TryPos2[1] = Quadat0_0[1].GetRotated(RotationYRad);
        *TryPos2[2] = Quadat0_0[2].GetRotated(RotationYRad);
        *TryPos2[3] = Quadat0_0[3].GetRotated(RotationYRad);

        TryPos2[0]->operator+=({ pos.X, pos.Y });
        TryPos2[1]->operator+=({ pos.X, pos.Y });
        TryPos2[2]->operator+=({ pos.X, pos.Y });
        TryPos2[3]->operator+=({ pos.X, pos.Y });
    }





    QuadBufferPtr->SetData(TryPos[0], color, TC[0], textureindex); QuadBufferPtr++;
    QuadBufferPtr->SetData(TryPos[1], color, TC[1], textureindex); QuadBufferPtr++;
    QuadBufferPtr->SetData(TryPos[2], color, TC[2], textureindex); QuadBufferPtr++;
    QuadBufferPtr->SetData(TryPos[3], color, TC[3], textureindex); QuadBufferPtr++;

    IndexCount += 6;
#ifdef ShowRunTimeStats
    DataStats.QuadCount++;
#endif // ShowRunTimeStats
}
void OpenGlRender::_DrawQuads2d(RenderRunTime2d::DrawData& _DrawData)
{
    std::sort(_DrawData.Quad2d.begin(), _DrawData.Quad2d.end(), draworder_Sort());
    for (size_t i = 0; i < _DrawData.Quad2d.size(); i++)
    {
        auto& Item = _DrawData.Quad2d[i];
        _DrawQuad2d(Item);
    }
}

RenderAPIEnd