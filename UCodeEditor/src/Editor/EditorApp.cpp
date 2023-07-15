#include "EditorApp.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/Camera2d.hpp"

#include <UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp>
#include "Helper/AppFiles.hpp"
#include <UCodeRunTime/ULibrarys/Loger.hpp>
EditorStart
EditorApp::EditorApp() : _App()
{
    
}

EditorApp::~EditorApp()
{
    
}

void EditorApp::Run(const String& ProjPath)
{
   UCODE_ENGINE_LOG("Hello World");


    UCode::RenderAPI::WindowData winData;
    winData.width = 800;
    winData.height = 540;
    winData.WindowName = "EditorApp";
    winData.SetUpInputs = false;
    //winData.UpdateAppIfInputUpdate = true; 
    
    UCode::GameFilesData AppFilesData;

    _App.Init(winData, AppFilesData);

    auto RunTime = _App.Get_RunTime();
    auto AppEntity = RunTime->NewEntityOnRunTimeScene();
    UCode::Camera2d* APPCam = AppEntity->AddCompoent<UCode::Camera2d>();
    EditorAppCompoent* APP = AppEntity->AddCompoent<EditorAppCompoent>();
    

    APP->Init(ProjPath);
    

    
    auto Lib = RunTime->Get_Library();
    auto Render = _App.Get_Render();
    auto Window = Render->Get_RenderAPI();
    AppFiles::AsynGetTexture(AppFiles::texture::AppIcon)
        .OnCompletedOnMainThread(
            [Window](UCode::Texture* Tex)
            {
                Window->SetWindowIcon(*Tex);
            });
   


    _App.Run();
}

EditorEnd
