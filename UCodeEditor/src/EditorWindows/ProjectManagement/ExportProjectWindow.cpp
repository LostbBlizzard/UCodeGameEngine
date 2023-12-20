#include "ExportProjectWindow.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "Helper/FileHelper.hpp"
#include "Helper/Tasks.hpp"
EditorStart
ExportProjectWindow::ExportProjectWindow(const NewEditorWindowData& windowdata)
	:EditorWindow(windowdata)
{
}
ExportProjectWindow::~ExportProjectWindow()
{

}

const ExportProjectWindow::PlatformsData PlatformsData_[] =
{
	{"Windows",ExportProjectWindow::Platforms::Windows,AppFiles::sprite::Windows_Platform,"This Makes An Windows executable"},
	{"Linux",ExportProjectWindow::Platforms::Linux_Standalone,AppFiles::sprite::Linux_Platform,"This Makes An Linux executable"},
	{"MaxOS",ExportProjectWindow::Platforms::Mac,AppFiles::sprite::Mac_Platform,"This Makes An MaxOS executable"},

    {"WebGL",ExportProjectWindow::Platforms::WebGL,AppFiles::sprite::Web_Platform,"This Makes An WebGL App."},
	{"Android",ExportProjectWindow::Platforms::Android,AppFiles::sprite::Android_Platform,"This Makes An Android Apk."},
	{"IOS",ExportProjectWindow::Platforms::iOS,AppFiles::sprite::IOS_Platform,"This Makes An IOS App."},


	{"Dedicated Server",ExportProjectWindow::Platforms::Dedicated_Server,AppFiles::sprite::Sever_Platform,"This Makes An Windowless App."},
	{"Custom",ExportProjectWindow::Platforms::Custom,AppFiles::sprite::Uility_image,"Use a Custom Exporter"},
	{"UCodeEditor",ExportProjectWindow::Platforms::UCodeEditor,AppFiles::sprite::UCodeEditor_Platform,"This Makes An UPugin"},
};
constexpr size_t PlatfromSize = sizeof(PlatformsData_) / sizeof(PlatformsData_[0]);


bool IsDone = false;

void ExportProjectWindow::UpdateWindow()
{
	bool _Building = RuningTasksInfo::HasTaskRuning(RuningTask::Type::BuildingProject);
	if (_Building)
	{
		
		if (IsDone)
		{
		
			ImGui::Text("Building Project...");
			//if (V == std::future_status::ready)
			{
				auto ProjectInfo = Get_ProjectData();
				auto OutputPath = ProjectInfo->GetOutDir();
				FileHelper::OpenPathinFiles(OutputPath);
				RuningTasksInfo::ReMoveTask(RuningTask::Type::BuildingProject);
			}
		}
		else
		{
			ImGui::Text("Please wait The Project Is Being Bult in a other window");
		}

		ImGuIHelper::ShowOnMiddleLoadingIndicatorCircle("Loading");

		return;
	}
	if (LookingAtPlatform == nullptr)
	{
		LookingAtPlatform = Get_PlatfromData(Platforms::ThisPlatform);
	}

	ImGui::Columns(2);

	ImGui::Text("Platforms");

	for (size_t i = 0; i < PlatfromSize; i++)
	{
		const auto& Item = PlatformsData_[i];
		bool IsOn = LookingAtPlatform->Value == Item.Value;

		
		String itemid = "##" + (String)Item.Name;
		UCode::Sprite* Tex;

		
		ImGuIHelper::Image(Item.Sprite, {20,20});
		ImGui::SameLine();
		if (ImGui::Selectable(itemid.c_str(), LookingAtPlatform->Value == Item.Value))
		{
			LookingAtPlatform = &Item;
		}

		ImGui::SameLine();
		ImGui::Text(Item.Name);
		

	}

	ImGui::NextColumn();

	if (LookingAtPlatform)
	{
		switch (LookingAtPlatform->Value)
		{
		case Platforms::Windows:ShowWindowsExportSeting();break;
		case Platforms::Linux_Standalone:ShowLinuxExportSeting(); break;
		case Platforms::Mac:ShowMacOsExportSeting(); break;

		case Platforms::WebGL:ShowWebExportSeting(); break;
		case Platforms::Android:ShowAndroidExportSeting(); break;
		case Platforms::iOS:ShowIosExportSeting(); break;

		case Platforms::Dedicated_Server:ShowSeverExportSeting(); break;
		case Platforms::UCodeEditor:ShowUCodeEditorExportSeting();break;
		case Platforms::Custom:ShowCustomExportSeting(); break;
		default:
			UCodeGEUnreachable();
			break;
		}
	}
}

EditorWindowData ExportProjectWindow::GetEditorData()
{
	return EditorWindowData("ExportProjectWindow", MakeWin);
}

const ExportProjectWindow::PlatformsData* ExportProjectWindow::Get_PlatfromData(Platforms Value)
{
	for (size_t i = 0; i < PlatfromSize; i++)
	{
		const auto& Item = PlatformsData_[i];
		if (Item.Value == Value)
		{
			return &Item;
		}
	}
	return nullptr;
}

EditorWindow* ExportProjectWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new ExportProjectWindow(windowdata);
}
void ExportProjectWindow::ShowWindowsExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Windows_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Windows");

	WindowsBuildSetings& Info = WinSettings;

	static const Vector<ImGuIHelper::EnumValue<WindowsBuildSetings::Architecture>> ArchitectureValues
	{ 
		#if UCodeGEWindows
		{"current cpu",WindowsBuildSetings::Architecture::Native},
		#endif
		{"x86_64",WindowsBuildSetings::Architecture::x86_64},
		{"x86",WindowsBuildSetings::Architecture::x86},
		
		//{"Arm64",WindowsBuildSetings::Architecture::Arm64},
		//{"Arm",WindowsBuildSetings::Architecture::Arm32},
	};

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGuIHelper::BoolEnumField("DebugBuild", Info.DebugBuild);

	ImGuIHelper::EnumField("Architecture", Info.architecture,ArchitectureValues);

	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{
		auto ProjectInfo = Get_ProjectData();
		auto AssetsPath = ProjectInfo->GetAssetsDir();
		auto OutputPath = ProjectInfo->GetOutDir();
		auto Threads = UCode::Threads::Get(Get_GameLib());
		buildSytem.Setings._OutDir = OutputPath;

		BuildSetings::SettingsType Copy = Info;
		SetBuildData(AssetsPath, ProjectInfo, Copy);
		
		RuningTask V;
		V.TaskType = RuningTask::Type::BuildingProject;
		RuningTasksInfo::AddTask(V);

		Delegate<void> Func = [buildSytem = &buildSytem]()
		{
			buildSytem->BuildProject();
		};

		//_Task = Threads->AddTask_t(UCode::TaskType::FileIO,std::move(Func), {});
		Func();

		IsDone = true;
	}

	
	
}

void ExportProjectWindow::ShowLinuxExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Linux_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Linux");

}

void ExportProjectWindow::ShowMacOsExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Mac_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("MaxOS");
}

void ExportProjectWindow::ShowWebExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Web_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Webgl");

	WebBuildSetings& Info = webSettings;


	ImGui::BeginDisabled(true);
	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{
		auto ProjectInfo = Get_ProjectData();
		auto AssetsPath = ProjectInfo->GetAssetsDir();
		auto OutputPath = ProjectInfo->GetOutDir();
		auto Threads = UCode::Threads::Get(Get_GameLib());
		buildSytem.Setings._OutDir = OutputPath;

		String AddedOnOutDir = "Web";
		{

		}
		buildSytem.Setings._OutDir /= AddedOnOutDir;


		//auto Copy = Info;
		//SetBuildData(AssetsPath, ProjectInfo, AddedOnOutDir, Copy);

		RuningTask V;
		V.TaskType = RuningTask::Type::BuildingProject;
		RuningTasksInfo::AddTask(V);

		Delegate<void> Func = [buildSytem = &buildSytem]()
		{
			buildSytem->BuildProject();
		};
	
		_Task = Threads->AddTask_t(
			UCode::TaskType::FileIO,
			std::move(Func), {});

	}
	ImGui::EndDisabled();

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
}

void ExportProjectWindow::SetBuildData(UCodeEditor::Path& AssetsPath, UCodeEditor::RunTimeProjectData* ProjectInfo,BuildSetings::SettingsType Info)
{
	buildSytem.Setings._InputDir = AssetsPath;
	buildSytem.Setings.TemporaryPlatfromPath = ProjectInfo->GetCachedDir().native() + Path(Path("Build")).native();
	buildSytem.Setings.TemporaryGlobalPath = ProjectInfo->GetCachedDir().native() + Path(Path("Build") / Path("Global")).native();
	buildSytem.Setings._OutName = ProjectInfo->Get_ProjData()._ProjectName;
	buildSytem.Setings.Settings = Info;
}

void ExportProjectWindow::ShowAndroidExportSeting()
{

	ImGuIHelper::Image(AppFiles::sprite::Android_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Android");

	AndroidBuildSetings& Info = androidSettings;

	
	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{
		auto ProjectInfo = Get_ProjectData();
		auto AssetsPath = ProjectInfo->GetAssetsDir();
		auto OutputPath = ProjectInfo->GetOutDir();
		auto Threads = UCode::Threads::Get(Get_GameLib());
		buildSytem.Setings._OutDir = OutputPath;

		String AddedOnOutDir = "Android";
		{

		}
		buildSytem.Setings._OutDir /= AddedOnOutDir;


		//auto Copy = Info;
		//SetBuildData(AssetsPath, ProjectInfo, AddedOnOutDir, Copy);

		RuningTask V;
		V.TaskType = RuningTask::Type::BuildingProject;
		RuningTasksInfo::AddTask(V);

		Delegate<void> Func = [buildSytem = &buildSytem]()
		{
			buildSytem->BuildProject();
		};

		_Task = Threads->AddTask_t(
			UCode::TaskType::FileIO,
			std::move(Func), {});
	}
	

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
}

void ExportProjectWindow::ShowIosExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::IOS_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Ios");
}

void ExportProjectWindow::ShowSeverExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Sever_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Sever");

	ImGui::BeginDisabled(true);
	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{

	}
	ImGui::EndDisabled();
}

void ExportProjectWindow::ShowUCodeEditorExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::UCodeEditor_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("UCodeEditor");

	ImGui::BeginDisabled(true);
	if (ImGui::Button("Export",{ImGui::GetContentRegionAvail().x,20}))
	{

	}
	ImGui::EndDisabled();
}

void ExportProjectWindow::ShowCustomExportSeting()
{
}

EditorEnd
