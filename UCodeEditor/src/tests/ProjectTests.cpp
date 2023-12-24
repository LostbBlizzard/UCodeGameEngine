#include "ProjectTests.hpp"

#include "BuildSytems/BuildSytemManger.hpp"
#include "UCodeRunTime/CoreSystems/StandardAssetLoader.hpp"
#include "UCodeRunTime/App.hpp"

EditorTestStart


Path outputpath = "tests/output";
Path outputprojectpath = "tests/output/project";
Path projectpaths = "tests/projects";

struct AppRuner
{
public:
	struct AppRunerCompoent :public UCode::Compoent
	{
		AppRuner* _This;

		AppRunerCompoent(UCode::Entity* entity):Compoent(entity,nullptr)
		{

		}

		void Start() override
		{
			_This->OnStart(*_This);
		}

		void Update() override
		{
			_This->OnUpdate(*_This);
		}
	};

	//should be practically identical to UCodeApp 
	//everything is lazily loaded to everything it should be ok
	bool Run(const Path& gamefilepath)
	{
		if (UCode::GameFilesData::ReadFileKeepOpen(gamefilepath, Buffer, GameData))
		{
			UCode::UApp app;
			app.Init();
			auto runtime = app.Get_RunTime();
			auto Library = runtime->Get_Library_Edit();

			UCode::GameFiles* gamefiles = UCode::GameFiles::Init(Library, GameData);
			gamefiles->SetFileBuffer(std::move(Buffer));
			UCode::AssetManager* manger = UCode::AssetManager::Get(Library);
			manger->GetDataFromGameFiles(gamefiles);

			UCode::StandardAssetLoader AssetLoader;
			AssetLoader.Init(gamefiles, manger);

			manger->Set_AssetLoader(&AssetLoader);

			UID sceneID = gamefiles->Get_FilesData().SceneToLoadOnPlay;
			auto r = manger->LoadAsset(sceneID);
			if (r.has_value())
			{
				auto& ScencData = r.value().Get_Value()->GetAssetAs<UCode::ScencAsset>()->_Base;
				auto scenc = UCode::Scene2dData::LoadScene(runtime, ScencData);

				scenc->NewEntity()->AddCompoent<AppRunerCompoent>();

				app.Run();

				return RetState.value_or(false);
			}
			else
			{
				return false;
			}
		}
		return false;
	}

	float MaxTimeOutSec = 15;
	std::function<void(AppRuner& runer)> OnStart;
	std::function<void(AppRuner& runer)> OnUpdate;


	UCode::GameFilesData GameData;
	UCode::FileBuffer Buffer;
	Optional<bool> RetState;
};

struct variantmode
{
	BuildSetings::SettingsType type;
	String str;
};
Vector<variantmode> GetThisOSSettingsV()
{
	#if UCodeGEWindows
	WindowsBuildSetings::Architecture arc = sizeof(void*) == 4 ? WindowsBuildSetings::Architecture::x86 : WindowsBuildSetings::Architecture::x86_64;

	WindowsBuildSetings debug;
	debug.DebugBuild = true;

	WindowsBuildSetings publish;
	publish.DebugBuild = false;

	return { {std::move(debug),"Debug"},{std::move(publish),"Publish"}};
	#else
	UCodeGEUnreachable();
	#endif
}

const Vector<variantmode>& GetThisOSSettings()
{
	thread_local Optional< Vector<variantmode>> val;
	if (!val.has_value())
	{
		val = GetThisOSSettingsV();
	}
	return val.value();
}
bool Project_Hello()
{
	auto& lists = GetThisOSSettings();


	for (auto& Item : lists) 
	{
		Path Projectpath = projectpaths / "Hello";
		Path Projectout = outputprojectpath / "Hello";

		BuildSytemManger v;
		v.Setings.Settings = Item.type;
		v.Setings._InputDir = Projectpath / "Assets";
		v.Setings._OutDir = Projectout;


		const auto r = v.BuildProject();
	
		bool prjectbuild = r.IsValue();

		if (prjectbuild)
		{
			auto& MainOut = r.GetValue();

			AppRuner runer;
			UCode::Entity* playerentity = nullptr;
			runer.OnStart = [&](AppRuner& _this) -> void
			{
					int a = 0;
			};
			runer.OnUpdate = [&](AppRuner& _this) -> void
			{

					int a = 0;
			};

			return runer.Run(MainOut);
		}
		else
		{
			return false;
		}
	}
}

EditorTestEnd