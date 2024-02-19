#include "ExportProjectWindow.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "Helper/FileHelper.hpp"
#include "Helper/Tasks.hpp"
#include <UCodeRunTime/ULibrarys/Loger.hpp>
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


	//{"Dedicated Server",ExportProjectWindow::Platforms::Dedicated_Server,AppFiles::sprite::Sever_Platform,"This Makes An Windowless App."},
	//{"Custom",ExportProjectWindow::Platforms::Custom,AppFiles::sprite::Uility_image,"Use a Custom Exporter"},
	{"UCodeEditor",ExportProjectWindow::Platforms::UCodeEditor,AppFiles::sprite::UCodeEditor_Platform,"This Makes An UPugin"},
};
constexpr size_t PlatfromSize = sizeof(PlatformsData_) / sizeof(PlatformsData_[0]);



void ExportProjectWindow::UpdateWindow()
{
	bool _Building = RuningTasksInfo::HasTaskRuning(RuningTask::Type::BuildingProject);
	if (_Building)
	{	
		ImGuIHelper::Text((StringView("Project Is Being Exported")));

		ImGuIHelper::ShowOnMiddleLoadingIndicatorCircle("Loading");

		return;
	}
	if (LookingAtPlatform == nullptr)
	{
		LookingAtPlatform = Get_PlatfromData(Platforms::ThisPlatform);
	}

	ImGui::Columns(2);

	ImGuIHelper::Text(StringView("Platforms"));

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
		ImGuIHelper::Text(StringView(Item.Name));
		

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

void OnBuildProjecDone(BuildSytemManger::BuildRet r)
{
	RuningTasksInfo::ReMoveTask(RuningTask::Type::BuildingProject);
	if (r.IsError())
	{
		auto& error = r.GetError();

		String Msg;
		for (auto& Item : error.Errors)
		{
			Msg.clear();

			Msg += "Export Error:";
			if (Item.filepath.has_value()) 
			{
				Msg += "(" + Item.filepath.value().generic_string();
				if (Item.lineNumber.has_value())
				{
					Msg += ":";
					Msg += std::to_string(Item.lineNumber.value());
				}
				Msg += "):";
			}
			Msg += Item.message;

			UCodeGEError(Msg);
		}
	}
	else
	{
		FileHelper::OpenPathinFiles(r.GetValue().parent_path());
	}

}

void ExportProjectWindow::ShowWindowsExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Windows_Platform, { 20,20 });
	ImGui::SameLine();
	ImGuIHelper::Text(StringView("Windows"));

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

		Delegate<void> Func = [Threads,buildSytem = &buildSytem]()
		{
			  auto r = buildSytem->BuildProject();
		
			  
			  Delegate<void> Func2 = [r]()
				  {
					  OnBuildProjecDone(r);
				  };
			  Threads->AddTask_t(UCode::TaskType::Main, std::move(Func2), {});
		};

		_Task = Threads->AddTask_t(UCode::TaskType::FileIO, std::move(Func), {});
	}

	ImGuIHelper::BoolEnumField("DebugBuild", Info.DebugBuild);

	ImGuIHelper::EnumField("Architecture", Info.architecture, ArchitectureValues);
}

void ExportProjectWindow::ShowLinuxExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Linux_Platform, { 20,20 });
	ImGui::SameLine();
	ImGuIHelper::Text(StringView("Linux"));

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::BeginDisabled(true);
	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{


	}
	ImGui::EndDisabled();
}

void ExportProjectWindow::ShowMacOsExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Mac_Platform, { 20,20 });
	ImGui::SameLine();
	ImGuIHelper::Text(StringView("MaxOS"));
	
	
	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	ImGui::BeginDisabled(true);
	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{


	}
	ImGui::EndDisabled();
}

void ExportProjectWindow::ShowWebExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Web_Platform, { 20,20 });
	ImGui::SameLine();
	ImGuIHelper::Text(StringView("Webgl"));

	WebBuildSetings& Info = webSettings;

	ImGui::Dummy(ImVec2(0.0f, 20.0f));

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
	buildSytem.Setings.TemporaryPlatfromPath = ProjectInfo->GetCachedDir().native() + Path(Path("build")).native();
	buildSytem.Setings.TemporaryGlobalPath = ProjectInfo->GetCachedDir().native() + Path(Path("build") / Path("global")).native();
	buildSytem.Setings._OutName = ProjectInfo->Get_ProjData()._ProjectName;
	buildSytem.Setings.Settings = Info;
}

void ExportProjectWindow::ShowAndroidExportSeting()
{

	ImGuIHelper::Image(AppFiles::sprite::Android_Platform, { 20,20 });
	ImGui::SameLine();
	ImGuIHelper::Text(StringView("Android"));

	AndroidBuildSetings& Info = androidSettings;

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	
	ImGui::BeginDisabled(true);
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
	ImGui::EndDisabled();

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
}

void ExportProjectWindow::ShowIosExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::IOS_Platform, { 20,20 });
	ImGui::SameLine();
	ImGuIHelper::Text(StringView("Ios"));
	
	ImGui::Dummy(ImVec2(0.0f, 20.0f));

	ImGui::BeginDisabled(true);
	if (ImGui::Button("Export", { ImGui::GetContentRegionAvail().x,20 }))
	{
	}
	ImGui::EndDisabled();
}

void ExportProjectWindow::ShowSeverExportSeting()
{
	ImGuIHelper::Image(AppFiles::sprite::Sever_Platform, { 20,20 });
	ImGui::SameLine();
	ImGuIHelper::Text(StringView("Sever"));

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	
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
	ImGuIHelper::Text(StringView("UCodeEditor"));

	ImGui::Dummy(ImVec2(0.0f, 20.0f));
	
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
