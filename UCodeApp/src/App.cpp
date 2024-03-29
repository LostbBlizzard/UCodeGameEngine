#include "App.hpp"
#include "UCodeRunTime/App.hpp"

#include "UCodeRunTime/CoreSystems/GameFiles.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
#include "UCodeRunTime/CoreSystems/StandardAssetLoader.hpp"
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
    

    UCode::Loger::InitCheck();
    //try
    {
        UCode::Path GameDataPath = (UCode::Path)GameFilesData::FileDataName;
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

            
            Vector<UID> keeploaded;
            Vector<Assetptr> keeploadassets;
            {
                auto bytes = gamefiles->ReadGameFileAsBytes(UCode::GameFilesData::GetUCodeDir() / UCode::StandardAssetLoader::LoadOnStart::FileWithDot);
                StandardAssetLoader::LoadOnStart V;
                UDeserializer serializer;
                serializer.SetBytes(bytes.AsView());
                
                V.Deserialize(serializer);

                keeploaded = std::move(V._LoadList);
            }
            keeploadassets.reserve(keeploaded.size());

            GameRunTime::SetCurrent(runtime);

            for (auto& Item : keeploaded)
            {
                keeploadassets.push_back(manger->LoadAsset(Item).value());
            }
            
            if (r.has_value())
            {
                auto senceop = r.value().Get_Value()->GetAssetAs<ScencAsset>();

                if (senceop.has_value())
                {
                    auto& ScencData = senceop.value()->_Base;
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
        else
        {
            return EXIT_FAILURE;
        }
    }
    //catch (const std::exception& ex)
    {
       // UCode::Loger::Log((UCode::String)"App crashed :" + (UCode::String)ex.what(), UCode::LogType::Fatal);


        #ifdef DEBUG
        while (true) {}
        #endif // DEBUG


        return EXIT_FAILURE;
    }
}
