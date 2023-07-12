#include "UserSettingsWindow.hpp"
#include "Helper/UserSettings.hpp"
#include "Helper/ImGuIHelper.hpp"
EditorStart
UserSettingsWindow::UserSettingsWindow(const NewEditorWindowData& windowdata) : EditorWindow(windowdata)
{

}
UserSettingsWindow::~UserSettingsWindow()
{

}
void UserSettingsWindow::OnSaveWindow(USerializer& Input)
{
	UserSettings::SaveSettings();
}


String RunableFilesExt[] = {".exe"};

constexpr size_t RunableFilesExtCount = sizeof(RunableFilesExt) / sizeof(RunableFilesExt[0]);




struct ArgumentExamples_t
{
	const char* EditorName;
	String Value;
};


const ArgumentExamples_t ArgumentExamples[] = { 
	
	{"Atom Editor",(String)UserSettings::FilePathArg + ":" + UserSettings::ColumnArg},
	{"JetBrains Rider",(String)UserSettings::ProjectPathArg + " -- line " + UserSettings::FileLineNumberArg + " " + UserSettings::FilePathArg},
	{"Visual Studio Code",(String)UserSettings::ProjectPathArg + " --goto " + UserSettings::FilePathArg + ":" + UserSettings::FileLineNumberArg + ":" + UserSettings::ColumnArg},
	{"Vim","+call cursor(" + (String)UserSettings::FileLineNumberArg + "," + UserSettings::ColumnArg + ")" + (String)UserSettings::FilePathArg},
};

constexpr size_t  ArgumentExamplesCount = sizeof(ArgumentExamples) / sizeof(ArgumentExamples[0]);

void UserSettingsWindow::UpdateWindow()
{
	auto& Settings = UserSettings::GetSettings();

	ImGuIHelper::InputPath("CodeEditor", Settings.CodeEditorPath, {}, ImGuIHelper::InputPathType::File, Span<String>(&RunableFilesExt[0], RunableFilesExtCount));
	
	
	ImGuIHelper::InputText("Arguments to Pass", Settings.OpenCodeEditorFileArg);

	if (IsShowingExampleArguments == false)
	{
		ImGui::SameLine();
		if (ImGui::Button("..."))
		{
			IsShowingExampleArguments = true;
		}
	}
	else
	{
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			IsShowingExampleArguments = false;
		}
		ImGui::Separator();
		
		for (size_t i = 0; i < ArgumentExamplesCount; i++)
		{
			auto& Item = ArgumentExamples[i];

			ImGui::PushID(&Item);

			ImGui::Text(Item.EditorName); ImGui::SameLine();

			bool B = ImGui::Button("Select"); ImGui::SameLine();

			ImGui::BeginDisabled(true);
			ImGuIHelper::InputText(":", (String&)Item.Value);
			ImGui::EndDisabled();

			
			if (B)
			{
				Settings.OpenCodeEditorFileArg = Item.Value;
				IsShowingExampleArguments = false;
			}

			ImGui::PopID();
		}
	}
}

EditorWindowData UserSettingsWindow::GetEditorData()
{
	return EditorWindowData("UserSettingsWindow", MakeWin);
}

EditorWindow* UserSettingsWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new  UserSettingsWindow(windowdata);
}

EditorEnd
