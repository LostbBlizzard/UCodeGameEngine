#include "OpenGlRender.hpp"
#include "OpenGLInput.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

#include <Imgui/imgui.h>
#include <Imgui/backends/imgui_impl_glfw.h>
#include <Imgui/backends/imgui_impl_opengl3.h>
#include <Imgui/imgui_internal.h>
// Ucode
#include <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include <UCodeRunTime/ULibrarys/InputManger/InputManger.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/ULibrarys/Math_Library.hpp>
RenderAPIStart

void OpenGlRender::glfwfiledrop(GLFWwindow* window, int count, const char** paths)
{
    int i;
    Vector<StringView> r;
    r.resize(count);
    for (i = 0; i < count; i++)
    {
        r[i] = StringView(paths[i]);
    }

    OpenGlRender::GetOpenGlRender(window)->_windowdata._filedroped(r);
}
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
            QuadVA = {};
            QuadVB = {};
            QuadIB = {};
            QuadBuffer = {};
            WhiteTexture = {};

            (*it)->EndRender();
            Classes.erase(it);
            return;
        }
    }
    UCodeGEUnreachable();
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
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // IMgui Update them
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_VISIBLE, true);
        glfwWindowHint(GLFW_FOCUSED, true);
        glfwWindowHint(GLFW_DECORATED, true);

        glfwWindowHint(GLFW_FLOATING, false);

        window = glfwCreateWindow(_windowdata.width, _windowdata.height, _windowdata.WindowName.c_str(), nullptr,
                                  (GLFWwindow*)_windowdata.shared_window);

        oldwindowWidth = -1;
        oldwindowHeight = -1;
        if (!window)
        {
            glfwTerminate();
        }

        glfwMakeContextCurrent(window);

        if (_windowdata._filedroped)
        {
            glfwSetDropCallback(window, glfwfiledrop);
        }

        if (_windowdata.ImGui_Init)
        {

            if (glewInit() != GLEW_OK)
            {
                throw std::runtime_error("glewInit failed");
            }
            // ImGui
            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO();
            (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            // io.IniFilename = nullptr;

            OpenGlRender::SetStyle_WoodLandDay();

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

            // ImGui
        }

        //
        GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GlCall(glEnable(GL_BLEND));

        if (windowdata.SetUpInputs)
        {
            Input::Set_InputCallBacks(window);
        }
    }
    else
    {
        if (!_windowdata.shared_window) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // IMgui Update them
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            glfwWindowHint(GLFW_VISIBLE, false);
            glfwWindowHint(GLFW_FOCUSED, false);
            glfwWindowHint(GLFW_DECORATED, false);

            glfwWindowHint(GLFW_FLOATING, false);

            window = glfwCreateWindow(_windowdata.width, _windowdata.height, _windowdata.WindowName.c_str(), nullptr,
                                      (GLFWwindow*)_windowdata.shared_window);
            glfwMakeContextCurrent(window);

            // if (_windowdata.ImGui_Init)
            {
                if (glewInit() != GLEW_OK)
                {
                    throw std::runtime_error("glewInit failed");
                }
            }
        }
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

void OpenGlRender::SetWindowIcon(const Sprite& sprite)
{
    if (window)
    {
        GLFWimage icon;
        icon.height = sprite.Get_Height();
        icon.width = sprite.Get_Width();

        auto texture = sprite.Get_texture();
        auto index = texture->GetPixelIndex(sprite.Get_Xoffset(),sprite.Get_Yoffset(), texture->Get_Width(), texture->Get_Height());

        icon.pixels = (unsigned char*)&texture->Get_ColorData()[index];

        glfwSetWindowIcon(window, 1, &icon);
    }

}
void OpenGlRender::_DrawOpenGl(RenderRunTime2d::DrawData& Data, Camera2d* cam)
{
    auto Width = cam->Get_WindowWidth();
    auto Height = cam->Get_WindowHeight();
    if (Width <= 0 && Height <= 0)
    {
        return;
    }

    cam->UpdateCamData();

    auto& Buffer = cam->Get_Buffer();

    if (!Buffer.Is_SwapChinTarget())
    {
        Buffer.Bind();
    }

    for (size_t i = 1; i < TextureSlots.size(); i++)
    {
        TextureSlots[i] = nullptr;
    }
    NextTextureSlot = 0;
    
    
    auto clearcolor = cam->Get_BackRoundClearColor();
    GlCall(glClearColor(clearcolor.R, clearcolor.G, clearcolor.B, clearcolor.A)); // Background color
    GlCall(glClear(GL_COLOR_BUFFER_BIT));
    GlCall(glViewport(0, 0, Width, Height));

    glm::mat4 Vp = cam->Get_ViewProjectionMatrix();
    glm::mat4 Tranform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    UseingShader->SetUniformMat4f("_ViewProj", Vp);
    UseingShader->SetUniformMat4f("_Transfrom", Tranform);

    auto drawdata = MakeDrawInstruction(Data);
    RunInstructions(drawdata,Data);
    
    Buffer.UnBind();
}
void OpenGlRender::SetStyle_WoodLandDay(ImGuiStyle* dst)
{
    // Hippoptamus gray
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
    if (rtime == nullptr || window == nullptr)
    {
        return;
    }

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
            if (_windowdata.CloseAppRequest.has_value())
            {
                _windowdata.CloseAppRequest.value()();
            }
            else 
            {
                RunTime->StopRunTime();
                IsRuning = false;
            }
        }
    }
}

void OpenGlRender::EndRender()
{

    Shutdown();
    if (_windowdata.ImGui_Init)
    {
        // ImGui
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    } // ImGui

    glfwDestroyWindow(window);

    if (_windowdata.ImGui_Init)
    {
        glfwTerminate();
    }
}

void OpenGlRender::glfwerror_callback(int error, const char* description)
{
    UCodeGEError("Glfw ERROR(" << error << "):" << description);
}

constexpr size_t MaxQuadCount = 1000;
constexpr size_t MaxQuadVertexCount = MaxQuadCount * 4;
constexpr size_t MaxQuadIndexCount = MaxQuadCount * 6;

constexpr size_t MaxLineCount = 100;
constexpr size_t MaxLineVertexCount = MaxLineCount * 2;



GLuint GLVersionV()
{
    GLint major = 0;
    GLint minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    if (major == 0 && minor == 0)
    {
        // Query GL_VERSION in desktop GL 2.x, the string will start with "<major>.<minor>"
        const char* gl_version = (const char*)glGetString(GL_VERSION);
        sscanf(gl_version, "%d.%d", &major, &minor);
    }
    return (GLuint)(major * 100 + minor * 10);
}

void OpenGlRender::Init()
{
    {
        auto v = GLVersionV();

        UCodeGELog("[ConsoleWindowSkip]:initializing OpenGl:" << v);
    }

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

    //Quads
    {
        QuadBuffer = std::make_unique<RenderRunTime2d::Vertex[]>(MaxQuadVertexCount);
        QuadVA = std::make_unique<VertexArray>();
        QuadVB = std::make_unique<VertexBuffer>(nullptr, MaxQuadVertexCount * sizeof(RenderRunTime2d::Vertex));

        VertexBufferLayout layout = VertexBufferLayout();

        layout.Push_float(3); // pos
        layout.Push_float(4); // color
        layout.Push_float(2); // TexCoord
        layout.Push_float(1); // TexIndex

        QuadVA->AddBuffer(QuadVB.get(), layout);

        Unique_array<u32> indices = std::make_unique<u32[]>(MaxQuadIndexCount);

        u32 offset = 0;
        for (auto i = 0; i < MaxQuadIndexCount; i += 6)
        {

            indices[i + 0] = 0 + offset; // 0
            indices[i + 1] = 1 + offset; // 1
            indices[i + 2] = 2 + offset; // 2

            indices[i + 3] = 2 + offset; // 2
            indices[i + 4] = 3 + offset; // 3
            indices[i + 5] = 0 + offset; // 0

            offset += 4;
        }

        QuadIB = std::make_unique<IndexBuffer>(indices.get(), MaxQuadIndexCount);
    }

    // white texture
    {
        auto rgb32data = (Color32)Color::White();
        WhiteTexture = std::make_unique<Texture>(1, 1, &rgb32data);
        WhiteTextureSlot = 0;
        TextureSlots[WhiteTextureSlot] = WhiteTexture.get();

        NextTextureSlot = 1;
    }
}
void OpenGlRender::Shutdown()
{
}

void OpenGlRender::BeginBatchQuad()
{
    QuadBufferPtr = QuadBuffer.get();
}

void OpenGlRender::EndBatchQuad()
{
    GLsizeiptr size = (GLsizeiptr)QuadBufferPtr - (GLsizeiptr)QuadBuffer.get();
    QuadVB->UpdateData(QuadBuffer.get(), size);
}

void OpenGlRender::FlushQuad()
{
    BindTextures();

    QuadVA->Bind();

    GlCall(glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr));

    IndexCount = 0;
    NextTextureSlot = 1;
}
void OpenGlRender::_DrawQuad2d(RenderRunTime2d::DrawQuad2dData& Data)
{

    if (IndexCount >= MaxQuadIndexCount || NextTextureSlot > GetMaxTextureSlots() - 1)
    {
        EndBatchQuad();
        FlushQuad();

        BeginBatchQuad();
    }


    const auto pos = Data.pos;
    const auto color = Data.color;
    const auto size = Data.size;


    auto info = GetTextureCords(Data.Spr);
    Texture* tex = info.Tex;
    auto& TC = info.Cords;

    i32 textureindex = GetTextureIndex(tex); 

    const f32 Zpos = 0;
    Vec3 TryPos[]{
        {pos.X, pos.Y, Zpos},
        {pos.X + size.X, pos.Y, Zpos},
        {pos.X + size.X, pos.Y + size.Y, Zpos},
        {pos.X, pos.Y + size.Y, Zpos} };
    { // rotation stuff here
        const f32 RotationY = Data.rotation.Y;
        const f32 RotationYRad = RotationY * Math::Deg2Rad;

        Vec2* TryPos2[]{
            (Vec2*)&TryPos[0],
            (Vec2*)&TryPos[1],
            (Vec2*)&TryPos[2],
            (Vec2*)&TryPos[3],
        };
        Vec2 Quadat0_0[4]{
            {},
            {size.X, 0.0f},
            {size.X, size.Y},
            {0.0f, size.Y},
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

    QuadBufferPtr->SetData(TryPos[0], color, TC[0], textureindex);
    QuadBufferPtr++;
    QuadBufferPtr->SetData(TryPos[1], color, TC[1], textureindex);
    QuadBufferPtr++;
    QuadBufferPtr->SetData(TryPos[2], color, TC[2], textureindex);
    QuadBufferPtr++;
    QuadBufferPtr->SetData(TryPos[3], color, TC[3], textureindex);
    QuadBufferPtr++;

    IndexCount += 6;
#ifdef ShowRunTimeStats
    DataStats.QuadCount++;
#endif // ShowRunTimeStats
}
OpenGlRender::DrawInstructions OpenGlRender::MakeDrawInstruction(RenderRunTime2d::DrawData& _DrawData)
{
    OpenGlRender::DrawInstructions r;
    std::sort(_DrawData.Quad2d.begin(), _DrawData.Quad2d.end(),
        [](const RenderRunTime2d::DrawQuad2dData& a, const RenderRunTime2d::DrawQuad2dData& b)
    {

        if (a.drawLayer == b.drawLayer)
        {
            return a.draworder < b.draworder;
        }
        else
        {
            return a.drawLayer < b.drawLayer;
        }
    }
    );

    std::sort(_DrawData.Lines2d.begin(), _DrawData.Lines2d.end(),
        [](const RenderRunTime2d::Draw2DLineData& a, const RenderRunTime2d::Draw2DLineData& b)
    {

        if (a.drawLayer == b.drawLayer)
        {
            if (a.draworder == b.draworder)
            {
                return a.Thickness < b.Thickness;
            }
            return a.draworder < b.draworder;
        }
        else
        {
            return a.drawLayer < b.drawLayer;
        }
    }
    );


    size_t quadstart = 0;
    size_t linestart = 0;

    Optional<DrawInstructions::DrawType> last;
    RenderRunTime2d::DrawLayer_t _LastDrawLayer;
    RenderRunTime2d::DrawOrder_t _LastDrawOrder;

    while (quadstart < _DrawData.Quad2d.size()
        || linestart < _DrawData.Lines2d.size())
    {
        RenderRunTime2d::DrawLayer_t layervalue = {};
        RenderRunTime2d::DrawOrder_t ordervalue = {};
        size_t newindex = 0;
        Optional<DrawInstructions::DrawType> TypeToPush;

        //This need to be updated
        if (quadstart < _DrawData.Quad2d.size())
        {
            auto& Item = _DrawData.Quad2d[quadstart];

            bool canbenext = true;
            
            
            if (canbenext) 
            {
                TypeToPush = DrawInstructions::DrawType::Ouad2d;
                newindex = quadstart;
                layervalue = Item.drawLayer;
                ordervalue = Item.draworder;
            }
        }
        if (linestart < _DrawData.Lines2d.size())
        {
            auto& Item = _DrawData.Lines2d[linestart];

            bool canbenext = true;
            
            if (canbenext) 
            {
                TypeToPush = DrawInstructions::DrawType::Line;
                newindex = linestart;
                layervalue = Item.drawLayer;
                ordervalue = Item.draworder;
            }
        }



        if (TypeToPush.has_value()) 
        {
            DrawInstructions::Instruction* NewIns;
            if (r.Instructions.size())
            {
                NewIns = &r.Instructions.back();

                if (NewIns->type != TypeToPush.value())
                {
                    r.Instructions.push_back({});
                    NewIns = &r.Instructions.back();
                    NewIns->type = TypeToPush.value();
                }
            }
            else
            {
                r.Instructions.push_back({});
                NewIns = &r.Instructions.back();
                NewIns->type = TypeToPush.value();
            }

            if (NewIns->span.Count == 0)
            {
                NewIns->span.StartIndex = newindex;
            }
            NewIns->span.Count++;

            switch (TypeToPush.value())
            {
            case DrawInstructions::DrawType::Ouad2d:quadstart++; break;
            case DrawInstructions::DrawType::Line:linestart++; break;
            default:UCodeGEUnreachable(); break;
            }
            last = TypeToPush.value();

            _LastDrawLayer = layervalue;
            _LastDrawOrder = ordervalue;
        }
        else
        {
            last = {};
        }
    }

    return r;
}
void OpenGlRender::RunInstructions(DrawInstructions& Instructions,RenderRunTime2d::DrawData& Data)
{
    for (auto& Item : Instructions.Instructions)
    {
        switch (Item.type)
        {
        case DrawInstructions::DrawType::Ouad2d:
        {
            Span<RenderRunTime2d::DrawQuad2dData> quads = spanof(Data.Quad2d, Item.span.StartIndex, Item.span.Count);


            BeginBatchQuad();
            for (auto& Item : quads)
            {
                _DrawQuad2d(Item);
            }
            EndBatchQuad();
            FlushQuad();

        }
        break;
        case DrawInstructions::DrawType::Line:
        {
            Span<RenderRunTime2d::Draw2DLineData> lines = spanof(Data.Lines2d, Item.span.StartIndex, Item.span.Count);
            
            BeginBatchLine();
            for (auto& Item : lines)
            {
                if (linewidth != Item.Thickness)
                {
                    linewidth = Item.Thickness;
                    glLineWidth(linewidth);
                }
                _DrawLine2d(Item);
            }
            EndBatchLine();
            FlushLine();
            
        }
        break;
        default:
            UCodeGEUnreachable();
            break;
        }

    }
}
void OpenGlRender::_DrawLine2d(RenderRunTime2d::Draw2DLineData& Data)
{
    if (LineCount >= MaxLineCount || NextTextureSlot > GetMaxTextureSlots() - 1)
    {
        EndBatchLine();
        FlushLine();

        BeginBatchLine();
    }

    auto info = GetTextureCords(nullptr);
    Texture* tex = info.Tex;
    auto& TC = info.Cords;

    auto index = GetTextureIndex(tex);

    QuadBufferPtr->SetData(Data.Start, Data.color, { 0,0 }, index);
    QuadBufferPtr++;
    QuadBufferPtr->SetData(Data.End, Data.color, { 1,1 }, index);
    QuadBufferPtr++;
    LineCount++;

}
void OpenGlRender::BeginBatchLine()
{
    QuadBufferPtr = QuadBuffer.get();
}

void OpenGlRender::EndBatchLine()
{
    size_t count = (size_t)QuadBufferPtr - (size_t)QuadBuffer.get();
    QuadVB->UpdateData(QuadBuffer.get(), count);
}

void OpenGlRender::FlushLine()
{
    BindTextures();

    QuadVA->Bind();

    size_t count = QuadBufferPtr  - QuadBuffer.get();
    GlCall(glDrawArrays(GL_LINES, 0, count));
    LineCount = 0;

    NextTextureSlot = 1;
}

OpenGlRender::TextureCod OpenGlRender::GetTextureCords(Sprite* spr)
{
    TextureCod r;

    auto& tex = r.Tex;
    auto& TC = r.Cords;
    if (spr)
    {
        tex = spr->Get_texture();
        f32 w = (f32)tex->Get_Width();
        f32 h = (f32)tex->Get_Height();

        f32 sx = (f32)spr->Get_Xoffset();
        f32 sy = (f32)spr->Get_Yoffset();
        f32 sw;
        f32 sh;

        if (spr->Get_Width() == Sprite::GetTexureSize)
        {
            sw = (f32)tex->Get_Width();
        }
        else
        {
            sw = (f32)spr->Get_Width();
        }

        if (spr->Get_Height() == Sprite::GetTexureSize)
        {
            sh = (f32)tex->Get_Height();
        }
        else
        {
            sh = (f32)spr->Get_Height();
        }

        // float A =w/sw

        TC[0] = { sx / w, sy / h };
        TC[1] = { (sx + sw) / w, sy / h };
        TC[2] = { (sx + sw) / w, (sy + sh) / h };
        TC[3] = { sx / w, (sy + sh) / h };
    }
    else
    {
        tex = nullptr;

        TC[0] = { 0.0f, 0.0f };
        TC[1] = { 1.0f, 0.0f };
        TC[2] = { 1.0f, 1.0f };
        TC[3] = { 0.0f, 1.0f };
    }

    
    return r;
}

i32 OpenGlRender::GetTextureIndex(Texture* tex)
{
    i32 textureindex = -1;

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

    if (textureindex == -1) // is not in Texture Slots.
    {
        textureindex = NextTextureSlot;
        TextureSlots[NextTextureSlot] = tex;
        NextTextureSlot++;
    }
    return textureindex;
}

void OpenGlRender::BindTextures()
{
    for (u32 i = 0; i < NextTextureSlot; i++)
    {
        auto& Item = TextureSlots[i];
        Item->TryUploadTexToGPU();

#define CanCompileTextureUnit !UCodeGEWasm
        bool CanUseTextureUnitAtRunTime = UCodeGEWindows;

#if !CanCompileTextureUnit
        CanUseTextureUnitAtRunTime = false;
#endif
        if (CanUseTextureUnitAtRunTime)
        {
#if CanCompileTextureUnit
            GlCall(glBindTextureUnit(i, Item->Get_RendererID()));
#endif
        }
        else
        {
            GlCall(glActiveTexture(GL_TEXTURE0 + i));
            GlCall(glBindTexture(GL_TEXTURE_2D, Item->Get_RendererID()));
        }
    }
}


RenderAPIEnd
