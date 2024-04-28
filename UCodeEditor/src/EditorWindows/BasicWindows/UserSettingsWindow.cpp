#include "UserSettingsWindow.hpp"
#include "Helper/UserSettings.hpp"
#include "Helper/ImGuIHelper.hpp"
#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlRender.hpp>
#include "Helper/NetworkHelper.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "Plugin/ImGuiHelper_UPlugin.hpp"
#include "UCodeRunTime/Rendering/InputHelper.hpp"
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
bool DrawKeyBind(KeyBinding& key)
{
	struct State
	{
		bool IsListening = false;
	};
	static String name;
	static State state;

	bool updated = true;
	name = key.ToString();
	ImGui::PushID(&key);
	bool r = ImGui::BeginCombo("##keybind", name.c_str(), ImGuiComboFlags_NoArrowButton);
	if (r)
	{
		if (state.IsListening)
		{
			if (ImGui::Button("Stop Listening"))
			{
				state.IsListening = false;
				updated = true;

			}

			for (size_t i = 0; i < ImGuiKey::ImGuiKey_COUNT; i++)
			{
				auto imkey = (ImGuiKey)i;

				if (ImGui::IsKeyDown(imkey))
				{
					key.key = UCode::RenderAPI::ImguiKeyToUCodeKey(imkey);
					state.IsListening = false;
					updated = true;
				}
			}
		

		}
		else
		{
			if (ImGui::Button("Start Listening"))
			{
				state.IsListening = true;
			}
			if (ImGui::Button("UnMapKey"))
			{
				key.key = UCode::InputKey::Null;
				updated = true;
			}
		}


		ImGui::EndCombo();
	}
	ImGui::PopID();
	return updated;
}
void UserSettingsWindow::UpdateWindow()
{
	auto& Settings = UserSettings::GetSettings();


	if (ImGui::BeginTabBar("##Editor_tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Application"))
		{
			{
				String str = "Application Verion : " + Version::CurrentUCodeVersion().ToString();
				ImGuIHelper::Text(str);
			}
			
			{
				String str = "Application Build : ";
				#if UCodeGEWindows
				str += "Windows";
				#endif
				
				#if UCodeGELinux
				str += "Linux";
				#endif
				
				#if UCodeGEMacOS
				str += "MacOS";
				#endif

				str += "";

				if (sizeof(size_t) == 4)
				{
					str += "32";
				}
				else
				{
					str += "64";
				}

				ImGuIHelper::Text(str);
			}

			if (Settings.allowautoudate == false)
			{
				thread_local bool isgetingit = false;
				thread_local Optional<Version> Newist;

				if (!Newist.has_value() && isgetingit == false)
				{
					isgetingit = true;
					UCode::Threads::Get_Threads()->AddTask(UCode::TaskType::NetWorkIO, []()
					{
						Newist = NetworkHelper::GetNewestVersion().value_or(Version::CurrentUCodeVersion());
						isgetingit = false;
					}, {});
				}

				if (isgetingit)
				{
					ImGuIHelper::Text(StringView("Checking for New Update..."));
				}
				else
				{
					auto v = Newist.value();
					if (v > Version::CurrentUCodeVersion())
					{
						String s = "New Update ";
						s += v.ToString();
						s += " is Available";

						ImGuIHelper::Text(StringView(s));
						ImGui::SameLine();
						if (ImGui::Button("Update"))
						{
							UserSettings::updateonclose = true;
							this->Get_App()->GetGameRunTime()->EndRunTime();
						}
					}
					else
					{
						ImGuIHelper::Text(StringView("No Update Available"));
						ImGui::SameLine();
						if (ImGui::Button("try again"))
						{
							Newist = {};
						}
					}

				}
			}

			ImGui::Separator();
		
			if (ImGui::Button("Show UserSetting.yaml in Files"))
			{
				FileHelper::OpenPathinFiles(std::filesystem::absolute(UserSettings::GetPath().parent_path()));
			}
			ImGuIHelper::BoolEnumField("Auto Update", Settings.allowautoudate);
			ImGuIHelper::BoolEnumField("DeleteGoesToTrash", Settings.DeleteGoesToTrash);

			ImGui::EndTabItem();

			ImGuiHelper_UPlugin::DrawUPluginFieldVector("Active Global Plugins",Settings.GloballyActivePlugins);


			ImGuIHelper::Text(StringView("Key Binds"));
			static String tep;
			if (ImGui::Button("Reset All KeyBinds"))
			{
				for (size_t i = 0; i < Settings.KeyBinds.size(); i++)
				{
					auto& data = UserSettings::KeyBindDataList[i];
					auto& key = Settings.KeyBinds[i];

					key = data.Default;
				}
			}
			for (size_t i = 0; i < Settings.KeyBinds.size(); i++)
			{
				auto& data = UserSettings::KeyBindDataList[i];
				auto& key = Settings.KeyBinds[i];

				tep = "   ";
				tep += data.KeyBindName;
				ImGuIHelper::Text(tep);
				ImGui::SameLine();

				DrawKeyBind(key);

				ImGui::SameLine();

				ImGui::PushID(&key);
				if (ImGui::Button("Reset"))
				{
					key = data.Default;
				}
				ImGui::PopID();
			}
		}	
		
		if (ImGui::BeginTabItem("CodeEditor"))
		{
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

					ImGuIHelper::Text(StringView(Item.EditorName)); ImGui::SameLine();

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
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Editor Style"))
		{
			enum class EditorSyle
			{
				WoodLandDay,
				WoodLandNight,
				GrayMode,
				DarkMode,
			};
			static  EditorSyle syle;
			static Vector<ImGuIHelper::EnumValue<EditorSyle>> SyleList =
			{
				{"Camo",EditorSyle::WoodLandDay},
				{"Military",EditorSyle::GrayMode},
				{"Army",EditorSyle::DarkMode},
			};
			if (ImGuIHelper::EnumField("Editor Built-In Syle", syle, SyleList))
			{
				switch (syle)
				{
				case EditorSyle::WoodLandDay:
					UCode::RenderAPI::OpenGlRender::SetStyle_WoodLandDay();
					break;
				case EditorSyle::WoodLandNight:
					UCode::RenderAPI::OpenGlRender::SetStyle_WoodLandNight();
					break;
				case EditorSyle::GrayMode:
					UCode::RenderAPI::OpenGlRender::SetStyle_Gray();
					break;
				case EditorSyle::DarkMode:
					UCode::RenderAPI::OpenGlRender::SetStyle_Dark();
					break;
				default:
					UCodeGEUnreachable();
					break;
				}
			}
		
			ImGui::ShowStyleEditor();


			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
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
