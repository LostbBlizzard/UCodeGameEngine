#include "ProjectWindow.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/CoreAssets.hpp"
#include "Plugin/ImGuiHelper_UPlugin.hpp"
EditorStart

ProjectWindow::ProjectWindow(const NewEditorWindowData& windowdata) : EditorWindow(windowdata)
{
	WindowName = "ProjectWindow";
}

ProjectWindow::~ProjectWindow()
{
	SaveProjectDatatFile();
}

void ProjectWindow::SaveProjectDatatFile()
{
	SaveCountDown = 0;
	auto ProjectData = Get_ProjectData();
	auto& ProjectData2 = ProjectData->Get_ProjData();
	ProjectData::WriteToFile(ProjectData->GetProjectDataPath(), ProjectData2, ProjectData2._SerializeType);
}

void ProjectWindow::UpdateWindow()
{

	if (SaveCountDown < 0)
	{
		SaveProjectDatatFile();
		SaveCountDown = 120;
	}
	else
	{
		auto Delta = Get_App()->GetGameRunTime()->Get_GameTime().UpateDelta;
		SaveCountDown -= Delta;
	}

	auto ProjectData = Get_ProjectData();
	auto& ProjectData2 = ProjectData->Get_ProjData();
	auto& Item = ProjectData2._ProjectName;
	 
	
	bool UpdateValue = false;

	if (ImGuIHelper::InputText("Project Name", Item))
	{
		UpdateValue = true;
	}

	static const ImGuIHelper::EnumValue<USerializerType> Values[] =
	{
		{"Bytes",USerializerType::BytesSafeMode},
		{"YAML",USerializerType::YAML},
	};
	constexpr size_t Values_Count = sizeof(Values) / sizeof(Values[0]);

	if (ImGuIHelper::EnumField("Serialization Mode", ProjectData2._SerializeType, Values, Values_Count))
	{
		UpdateValue = true;
	}

	{
		UCode::ScencPtr Ptr;
		Ptr = ProjectData2.StartScene;
		if (ImGuIHelper_Asset::AsssetField("Start Scene", Ptr))
		{
			UpdateValue = true;


			ProjectData2.StartScene = Ptr.Get_UID();
		}
	}

	ImGuiHelper_UPlugin::DrawUPluginFieldVector("Plugins", ProjectData2._Plugins);

	if (UpdateValue)
	{
		if (SaveCountDown > 15) {
			SaveCountDown = 15;
		}
	}
}
EditorWindowData ProjectWindow::GetEditorData()
{
	return EditorWindowData("ProjectWindow", MakeWin);
}
EditorWindow* ProjectWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new ProjectWindow(windowdata);
}
EditorEnd


