#include "EditorApp.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/Camera2d.hpp"

#include <UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp>
#include "Helper/AppFiles.hpp"
#include <UCodeRunTime/ULibrarys/Loger.hpp>

#if UCodeGEWindows
#include "Windows.h"
#include "WinUser.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
EditorStart
EditorApp::EditorApp() : _App()
{
    
}

EditorApp::~EditorApp()
{
    
}

void EditorApp::Run(const String& ProjPath)
{

    UCodeGELog("Hello World");


    UCode::RenderAPI::WindowData winData;
    winData.width = 800;
    winData.height = 540;
    winData.WindowName = "EditorApp";
    winData.SetUpInputs = false;
    winData.UpdateAppIfInputUpdate = true;
    static EditorAppCompoent* app = nullptr;
    winData._filedroped = [](Vector< StringView > paths)
        {
            app->OnFilesDropedOnWindow(paths);
        };

    UCode::GameFilesData AppFilesData;

    _App.Init(winData, AppFilesData);

    auto RunTime = _App.Get_RunTime();
    auto AppEntity = RunTime->NewEntityOnRunTimeScene();
    UCode::Camera2d* APPCam = AppEntity->AddCompoent<UCode::Camera2d>();
    EditorAppCompoent* APP = AppEntity->AddCompoent<EditorAppCompoent>();

    APP->SetWaitForInput = [this](bool V)
        {
            _App.Get_Render()->Get_RenderAPI()->SetWaitForEvents(V);
        };

    app = APP;

    APP->Init(ProjPath);

    auto Lib = RunTime->Get_Library_Edit();
    auto Render = _App.Get_Render();
    auto Window = Render->Get_RenderAPI();
    AppFiles::AsynGetTexture(AppFiles::texture::AppIcon)
        .OnCompletedOnMainThread(
            [Window](UCode::Texture* Tex)
            {
                Window->SetWindowIcon(*Tex);
                Tex->FreeFromCPU();
            }).Start();


            UCode::Threads::Get(Lib)->OnTaskSentToMain = [this, Render]()
                {
                    auto window = Render->Get_RenderAPI()->Get_Window();
#if UCodeGEWindows
                    auto handle = glfwGetWin32Window(window);
                    PostMessage(handle, WM_USER, 0, 0);
#endif 

                };

            _App.Run();
}

EditorEnd
