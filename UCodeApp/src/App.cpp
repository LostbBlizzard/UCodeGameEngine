#include "App.hpp"
#include "UCodeRunTime/App.hpp"

#include "UCodeRunTime/CoreBooks/GameFiles.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
#include "UCodeRunTime/CoreBooks/StandardAssetLoader.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/CoreAssets.hpp"
#include <filesystem>
using namespace UCode;
int App::main(int argc, char* argv[])
{
    #ifdef DEBUG
    while (true);//debuger beak point
    #endif // DEBUG


    UCode::Loger::InitCheck();

    try
    {
        #ifdef UCode_Build_Windows_OS

        UCode::Path Tep = argv[0];


        UCode::Path GameDataPath = Tep.parent_path() / (UCode::Path)GameFilesData::FileDataName;
        #endif
        GameFilesData GameData;
        FileBuffer Buffer;
        if (GameFilesData::ReadFileKeepOpen(GameDataPath, Buffer, GameData))
        {
            UCode::UApp app;
            app.Init();
            auto runtime = app.Get_RunTime();
            auto Library = runtime->Get_Library_Edit();

            GameFiles* gamefiles = GameFiles::Init(Library, GameData);
            gamefiles->SetFileBuffer(std::move(Buffer));
            AssetManager* manger = AssetManager::Get(Library);
            manger->GetDataFromGameFiles(gamefiles);

            StandardAssetLoader AssetLoader;
            AssetLoader.Init(gamefiles,manger);
            
            manger->Set_AssetLoader(&AssetLoader);
                
            UID sceneID = gamefiles->Get_FilesData().SceneToLoadOnPlay;
            auto r = manger->LoadAsset(sceneID);
            if (r.has_value())
            {
                auto& ScencData = r.value().Get_Value()->GetAssetAs<ScencAsset>()->_Base;
                Scene2dData::LoadScene(runtime, ScencData);

                app.Run();

                return EXIT_SUCCESS;
            }
            else
            {
                return EXIT_FAILURE;
            }
        }
        else
        {
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& ex)
    {
        UCode::Loger::Log((UCode::String)"App crashed :" + (UCode::String)ex.what(), UCode::LogType::Fatal);


        #ifdef DEBUG
        while (true) {}
        #endif // DEBUG


        return EXIT_FAILURE;
    }
}
