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
	if (_Task.valid()) 
	{
		_Task.wait();
	}
}

const ExportProjectWindow::PlatformsData PlatformsData_[] =
{
	{"Windows",ExportProjectWindow::Platforms::Windows,AppFiles::sprite::Windows_Platform,"This Makes An Windows Exe."},
    {"WebGL",ExportProjectWindow::Platforms::WebGL,AppFiles::sprite::Web_Platform,"This Makes An WebGL App."},
	{"Android",ExportProjectWindow::Platforms::Android,AppFiles::sprite::Android_Platform,"This Makes An Android Apk."},
	{"Dedicated Server",ExportProjectWindow::Platforms::Dedicated_Server,AppFiles::sprite::Sever_Platform,"This Makes An Windowless App."},
	{"UCodeEditor",ExportProjectWindow::Platforms::UCodeEditor,AppFiles::sprite::UCodeEditor_Platform,"This Makes An UPugin"},
};
constexpr size_t PlatfromSize = sizeof(PlatformsData_) / sizeof(PlatformsData_[0]);

void ExportProjectWindow::UpdateWindow()
{
	bool _Building = RuningTasksInfo::HasTaskRuning(RuningTask::Type::BuildingProject);
	if (_Building)
	{
		
		if (_Task.valid()) 
		{
			auto V = _Task.wait_for(std::chrono::seconds(0));

			ImGui::Text("Building Project...");
			if (V == std::future_status::ready)
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
		case Platforms::WebGL:ShowWebExportSeting(); break;
		case Platforms::Android:ShowAndroidExportSeting(); break;
		case Platforms::Dedicated_Server:
		case Platforms::UCodeEditor:ShowUCodeEditorExportSeting();break;
		default:break;
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
		#if UCode_Build_Windows_OS
		{"current cpu",WindowsBuildSetings::Architecture::Native},
		#endif
		{"x86_64",WindowsBuildSetings::Architecture::x86_64},
		{"x86",WindowsBuildSetings::Architecture::x86},
		
	};

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGuIHelper::BoolEnumField("DebugBuild", Info.DebugBuild);

	ImGuIHelper::EnumField("Architecture", Info.architecture,ArchitectureValues);

	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{
		auto ProjectInfo = Get_ProjectData();
		auto AssetsPath = ProjectInfo->GetAssetsDir();
		auto OutputPath = ProjectInfo->GetOutDir();
		auto Threads = UCode::BookOfThreads::Get(Get_GameLib());
		buildSytem.Setings._OutDir = OutputPath;

		String AddedOnOutDir = "windows";
	
		{
			if (Info.architecture == WindowsBuildSetings::Architecture::x86_64)
			{
				AddedOnOutDir += "-x86_64";
			}
			else
			{
				AddedOnOutDir += "-x86";
			}
			if (Info.DebugBuild)
			{
				AddedOnOutDir += "-debug";
			}
		}
		buildSytem.Setings._OutDir /= AddedOnOutDir;

		SetBuildData(AssetsPath, ProjectInfo, AddedOnOutDir, Info);
		
		RuningTask V;
		V.TaskType = RuningTask::Type::BuildingProject;
		RuningTasksInfo::AddTask(V);
		
		_Task = Threads->AddTask_t(
			UCode::BookOfThreads::TaskType::FileIO,
			&buildSytem,
			&BuildSytemManger::BuildProject);
		
	}

	
	
}

void ExportProjectWindow::ShowWebExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Web_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Webgl");

	WebBuildSetings& Info = WebSettings;

	
	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{
		auto ProjectInfo = Get_ProjectData();
		auto AssetsPath = ProjectInfo->GetAssetsDir();
		auto OutputPath = ProjectInfo->GetOutDir();
		auto Threads = UCode::BookOfThreads::Get(Get_GameLib());
		buildSytem.Setings._OutDir = OutputPath;

		String AddedOnOutDir = "Web";
		{

		}
		buildSytem.Setings._OutDir /= AddedOnOutDir;

		SetBuildData(AssetsPath, ProjectInfo, AddedOnOutDir, Info);

		RuningTask V;
		V.TaskType = RuningTask::Type::BuildingProject;
		RuningTasksInfo::AddTask(V);

		_Task = Threads->AddTask_t(
			UCode::BookOfThreads::TaskType::FileIO,
			&buildSytem,
			&BuildSytemManger::BuildProject);

	}

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
}

void ExportProjectWindow::SetBuildData(UCodeEditor::Path& AssetsPath, UCodeEditor::RunTimeProjectData* ProjectInfo, UCodeEditor::String& AddedOnOutDir, BuildSetings::SettingsType Info)
{
	buildSytem.Setings._InputDir = AssetsPath;
	buildSytem.Setings.TemporaryPlatfromPath = ProjectInfo->GetCachedDir().native() + Path(Path("build") / AddedOnOutDir).native();
	buildSytem.Setings.TemporaryGlobalPath = ProjectInfo->GetCachedDir().native() + Path(Path("build") / Path("global")).native();
	buildSytem.Setings._OutName = ProjectInfo->Get_ProjData()._ProjectName;
	buildSytem.Setings.Settings = Info;
}

void ExportProjectWindow::ShowAndroidExportSeting()
{

	ImGuIHelper::Image(AppFiles::sprite::Android_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Android");

	AndroidBuildSetings& Info = AndroidSettings;

	
	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20}))
	{
		auto ProjectInfo = Get_ProjectData();
		auto AssetsPath = ProjectInfo->GetAssetsDir();
		auto OutputPath = ProjectInfo->GetOutDir();
		auto Threads = UCode::BookOfThreads::Get(Get_GameLib());
		buildSytem.Setings._OutDir = OutputPath;

		String AddedOnOutDir = "Android";
		{

		}
		buildSytem.Setings._OutDir /= AddedOnOutDir;

		SetBuildData(AssetsPath, ProjectInfo, AddedOnOutDir, Info);

		RuningTask V;
		V.TaskType = RuningTask::Type::BuildingProject;
		RuningTasksInfo::AddTask(V);

		_Task = Threads->AddTask_t(
			UCode::BookOfThreads::TaskType::FileIO,
			&buildSytem,
			&BuildSytemManger::BuildProject);
	}
	

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
}

void ExportProjectWindow::ShowSeverExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Sever_Platform, { 20,20 });
	ImGui::SameLine();
	ImGui::Text("Sever");

	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{

	}
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

EditorEnd
