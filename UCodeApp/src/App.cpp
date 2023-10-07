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

    {
        /*
        UCode::USeverApp app;
        app.Init();
        auto runtime = app.Get_RunTime();
        auto Library = runtime->Get_Library_Edit();

        BookOfThreads* b = BookOfThreads::Get(Library);

        thread_local AsynTask MyTask;
        Delegate<int> func = []()
        {
            auto p = MyTask.GetProgress();
            std::cout << MyTask.GetProgress().AsPercent() << std::endl;
            return 5;
        };

        Delegate<int,int> func2 = [](int&& V)
        {
            std::cout << MyTask.GetProgress().AsPercent() << std::endl;
            return V + 1;
        };

        Delegate<void, int> func3 = [&app](int&& V)
        {
            std::cout << MyTask.GetProgress().AsPercent() << std::endl;

            app.Get_RunTime()->EndRunTime();
        };

        MyTask = b->AddTask_t(TaskType::Main, std::move(func), {})
            .ContinueCurrentThread<int>(std::move(func2))
            .ContinueCurrentThread(std::move(func3));

        app.Run();
    
        std::cout << MyTask.GetProgress().AsPercent() << std::endl;
        */
    }

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
