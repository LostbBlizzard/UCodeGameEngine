#include "ProjectWindow.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/CoreAssets.hpp"
#include "Plugin/ImGuiHelper_UPlugin.hpp"
#include "Helper/UserSettings.hpp"
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
	auto& settings = UserSettings::GetSettings();
	if (Get_App()->GetInputMode() == KeyInputMode::Window)
	{
		if (settings.IsKeybindActive(KeyBindList::ProjectWindow))
		{
			ImGui::SetWindowFocus();
			Get_App()->SetToNormal();
		}
	}

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

	bool UpdateValue = false;

	auto currentpos = ImGui::GetCursorPos();
	const float FieldHeight = ImGui::GetFrameHeight();
	const float FieldCount = 3;

	{

	
		const float ImageSize = FieldHeight * FieldCount;
		UCode::SpritePtr Ptr;
		Ptr = ProjectData2.GameIcon;
		if (ImGuIHelper_Asset::IconField("Game Icon", Ptr, { ImageSize,ImageSize }))
		{
			UpdateValue = true;
			ProjectData2.GameIcon = Ptr.Get_UID();
		}

	}

	{
		auto startdrawposx = FieldHeight * FieldCount + (ImGui::GetStyle().ItemSpacing.x * 3);

		ImGui::SetCursorPosY(currentpos.y);
		ImGui::SetCursorPosX(startdrawposx);
		if (ImGuIHelper::InputText("Project Name", ProjectData2._ProjectName))
		{
			UpdateValue = true;
		}

		ImGui::SetCursorPosX(startdrawposx);
		if (ImGuIHelper::InputText("Company Name", ProjectData2._CompanyName))
		{
			UpdateValue = true;
		}

		ImGui::SetCursorPosX(startdrawposx);
		auto str = ProjectData2._Version.ToString();
		if (ImGuIHelper::InputText("Game Version",str))
		{
			auto v = UC::Version::Parse(str);
			if (v.has_value()) 
			{
				ProjectData2._Version = v.value();
			}
			UpdateValue = true;
		}
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

	if (ImGuIHelper_Asset::AnyAsssetsField("Load On Start Up/Assets To Keep Loaded", ProjectData2._AssetsToKeepLoaded))
	{
		UpdateValue = true;
	}

	if (ImGuiHelper_UPlugin::DrawUPluginFieldVector("Plugins", ProjectData2._Plugins))
	{
		UpdateValue = true;
	}

	ImGuIHelper::Text(StringView("Modding Support"));
	ImGui::Indent();
	{
		if (ImGuIHelper::BoolEnumField("Allow Modding", ProjectData2.ModingSupport))
		{
			UpdateValue = true;
		}


		ImGui::BeginDisabled(!ProjectData2.ModingSupport);
		{

		}
		ImGui::EndDisabled();
	}
	ImGui::Unindent();

	ImGuIHelper::Text(StringView("Prefabs"));
	ImGui::Indent();
	{
			UCode::ScencPtr Ptr;
			UCode::ScencPtr Ptr2;
		{
			Ptr = ProjectData2.PrefabScene;
			if (ImGuIHelper_Asset::AsssetField("Prefab Editor Scene", Ptr))
			{
				UpdateValue = true;


				ProjectData2.PrefabScene = Ptr.Get_UID();
			}
		}

		{
			Ptr2 = ProjectData2.PrefabUIScene;
			if (ImGuIHelper_Asset::AsssetField("Prefab UI Editor Scene", Ptr2))
			{
				UpdateValue = true;


				ProjectData2.PrefabUIScene= Ptr2.Get_UID();
			}
		}

	}
	ImGui::Unindent();

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


