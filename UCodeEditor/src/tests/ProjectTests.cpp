#include "ProjectTests.hpp"

#include "BuildSytems/BuildSytemManger.hpp"
#include "UCodeRunTime/CoreSystems/StandardAssetLoader.hpp"
#include "UCodeRunTime/App.hpp"
#include "UCodeRunTime/ULibrarys/InputManger/InputManger.hpp"
EditorTestStart


Path outputpath = "tests/output";
Path outputprojectpath = "tests/output/project";
Path projectpaths = "tests/projects";

struct AppRuner
{
public:
	struct AppRunerCompoent :public UCode::Compoent
	{
		AppRuner* _This =nullptr;

		AppRunerCompoent(UCode::Entity* entity):Compoent(entity,nullptr)
		{

		}

		void Start() override
		{
			_This->OnStart(*_This, *this);
		}

		void Update() override
		{
			_This->OnUpdate(*_This,*this);
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

			run = runtime;

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
				auto sceneop = r.value().Get_Value()->GetAssetAs<UCode::ScencAsset>();
				if (sceneop.has_value())
				{
					auto& ScencData = sceneop.value()->_Base;
					auto scenc = UCode::Scene2dData::LoadScene(runtime, ScencData);

					auto apprun = scenc->NewEntity()->AddCompoent<AppRunerCompoent>();
					apprun->_This = this;

					app.Run();

					return RetState.value_or(false);
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		return false;
	}
	void CloseApp()
	{
		run->StopRunTime();
	}
	float MaxTimeOutSec = 15;
	std::function<void(AppRuner& runer, AppRunerCompoent& run)> OnStart;
	std::function<void(AppRuner& runer, AppRunerCompoent& run)> OnUpdate;

	UCode::GameRunTime* run =nullptr;
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

BuildSytemManger::BuildRet BuildProject(BuildSytemManger& v)
{
	BuildSytemManger::BuildRet r;
	{
		Path Projectpath = projectpaths / "Hello";
		Path Projectout = outputprojectpath / "Hello";

		UCode::GameFilesData AppFilesData;
		auto _run = std::make_unique<UCode::GameRunTime>();
		_run->Init();


		//setup Opengl Context
		auto _render = std::make_unique<UCode::RenderAPI::Render>();
		_render->PreInit();

		UCode::RenderAPI::WindowData windowdata;
		windowdata.GenNewWindow = false;
		windowdata.ImGui_Init = false;
		_render->Init(windowdata, _run.get());

		auto GameFiles = UCode::GameFiles::Init(_run->Get_Library_Edit(), AppFilesData);//This Can be Moved 

		AppFiles::Init(_run->Get_Library_Edit());
		r = v.BuildProject();

		_render->EndRender();
	}
	return r;
}
BuildSetings GetBuildSettingForProject(String projectname,const variantmode& Item)
{

	Path Projectpath = projectpaths / projectname;
	Path Projectout = outputprojectpath / projectname;


	BuildSetings v;
	v.Settings = Item.type;
	v._InputDir = Projectpath / "Assets";
	v._OutDir = Projectout / "out";
	v.TemporaryGlobalPath = Projectout / "global";
	v._OutName = projectname;
	v.TemporaryPlatfromPath = Projectout / "platform";

	return v;
}

bool Project_Hello()
{
	auto& lists = GetThisOSSettings();


	for (auto& Item : lists) 
	{

		BuildSytemManger v;
		v.Setings = GetBuildSettingForProject("Hello",Item);

		BuildSytemManger::BuildRet r = BuildProject(v);

		if (r.IsValue())
		{
			auto& MainOut = r.GetValue();

			AppRuner runer;
			UCode::Entity* playerentity = nullptr;
			UCode::Vec2 startpos = {};
			float time = 2;
			runer.OnStart = [&](AppRuner& _this, AppRuner::AppRunerCompoent& run) -> void
			{
					playerentity = run.Get_Scene()->Get_Entitys()[0].get();
					startpos = playerentity->worldposition2d();
					int a = 0;
			};
			runer.OnUpdate = [&](AppRuner& _this, AppRuner::AppRunerCompoent& run) -> void
			{
					auto in = UCode::InputManger::GetInput(run.GetGameRunTime());
					in->Input_API_SetInput(true, UCode::InputKey::W);
					
					time -= run.GetGameRunTime()->Get_GameTime().UpateDelta;
					if (time <= 0)
					{
						_this.RetState = startpos != playerentity->worldposition2d();
						_this.CloseApp();
					}
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