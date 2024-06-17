#include "UserSettingsWindow.hpp"
#include "Helper/UserSettings.hpp"
#include "Helper/ImGuIHelper.hpp"
#include <UCodeRunTime/Rendering/API_OpenGL/OpenGlRender.hpp>
#include "Helper/NetworkHelper.hpp"
#include "Editor/EditorAppCompoent.hpp"
#include "Plugin/ImGuiHelper_UPlugin.hpp"
#include "UCodeRunTime/Rendering/InputHelper.hpp"
#include "Imgui/imgui_internal.h"
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
		auto val = ImGui::GetContentRegionAvail().x;
		auto framey = ImGui::GetFrameHeight();
		auto buttionx = (val / 2) - ImGui::GetStyle().ItemSpacing.x;

		if (state.IsListening)
		{
					static Array< ImGuiKey, 9> skiplist = 
					{
						ImGuiKey::ImGuiKey_MouseLeft,
						ImGuiKey::ImGuiKey_MouseRight,
						ImGuiKey::ImGuiKey_MouseMiddle,
						ImGuiKey::ImGuiKey_LeftAlt,
						ImGuiKey::ImGuiKey_LeftShift,
						ImGuiKey::ImGuiKey_LeftCtrl,
						ImGuiKey::ImGuiKey_RightCtrl,
						ImGuiKey::ImGuiKey_RightShift,
						ImGuiKey::ImGuiKey_RightAlt,
					};

			if (ImGui::Button("Stop Listening",{buttionx,framey}))
			{
				state.IsListening = false;
				updated = true;

			}
			String currentkey = "";

			if (ImGui::IsKeyDown(ImGuiMod_Ctrl)){currentkey += "Ctrl+";}
			if (ImGui::IsKeyDown(ImGuiMod_Alt)){currentkey += "Alt+";}
			if (ImGui::IsKeyDown(ImGuiMod_Shift)){currentkey += "Shift+";}
			{
				for (size_t i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++)
				{
					auto imkey = (ImGuiKey)i;

					if (ImGui::IsKeyDown(imkey))
					{
						auto val = ImGui::GetKeyData(imkey);

						{
							auto val = (int)imkey;
							val &= ~ImGuiMod_Mask_;
							imkey = (ImGuiKey)val;
						}

						bool skip = false;
						for (auto& Item : skiplist)
						{
							if (imkey == Item)
							{
								skip = true;
								break;
							}
						}
						if (skip)
						{
							continue;
						}

						KeyBinding bind;
						bind.key = UCode::RenderAPI::ImguiKeyToUCodeKey(imkey);

						if (bind.key == UCode::InputKey::Null) { continue; }
						currentkey += bind.ToString();
						break;
					}
				}
			}

			ImGui::SameLine();
			ImGui::SetNextItemWidth(buttionx);
			ImGuIHelper::Text((String)currentkey);

			for (size_t i = ImGuiKey_NamedKey_BEGIN; i < ImGuiKey_NamedKey_END; i++)
			{
				auto imkey = (ImGuiKey)i;

				if (ImGui::IsKeyReleased(imkey))
				{
					auto val = ImGui::GetKeyData(imkey);

					{
						auto val = (int)imkey;
						val &= ~ImGuiMod_Mask_;
						imkey = (ImGuiKey)val;
					}

					bool skip = false;
					for (auto& Item : skiplist)
					{
						if (imkey == Item)
						{
							skip = true;
							break;
						}
					}
					if (skip)
					{
						continue;
					}

					key.key = UCode::RenderAPI::ImguiKeyToUCodeKey(imkey);

					if (key.key == UCode::InputKey::Null)
					{
						continue;
					}

					key.IsCtrl = ImGui::IsKeyDown(ImGuiMod_Ctrl);
					key.IsAlt = ImGui::IsKeyDown(ImGuiMod_Alt);
					key.IsShift = ImGui::IsKeyDown(ImGuiMod_Shift);

					state.IsListening = false;
					updated = true;
					break;
				}
			}
		

		}
		else
		{

			if (ImGui::Button("Start Listening",{buttionx,framey}))
			{
				state.IsListening = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Unmap Key",{buttionx,framey}))
			{
				key = KeyBinding();
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

    if (Get_App()->GetInputMode() == KeyInputMode::Window)
    {
        if (Settings.IsKeybindActive(KeyBindList::UserWindow)) 
		{
            ImGui::SetWindowFocus();
			Get_App()->SetToNormal();
        }
    }

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

			ImGuiHelper_UPlugin::DrawUPluginFieldVector("Active Global Plugins", Settings.GloballyActivePlugins);


			ImGuIHelper::Text(StringView("Key Binds"));
			static String tep;

			ImGui::SameLine();
			{
				auto val = ImGui::GetContentRegionAvail().x;

				auto buttionx = (val / 2) - ImGui::GetStyle().ItemSpacing.x;
				auto buttiony = ImGui::GetFrameHeight();
				if (ImGui::Button("Reset All KeyBinds", { buttionx,buttiony }))
				{
					for (size_t i = 0; i < Settings.KeyBinds.size(); i++)
					{
						auto& data = UserSettings::KeyBindDataList[i];
						auto& key = Settings.KeyBinds[i];

						key = data.Default;
					}
				}

				ImGui::SameLine();

				if (ImGui::Button("Set to Vim KeyBinds", { buttionx,buttiony }))
				{
					Settings.SetAsVimKeyBinds();
				}
			}
			bool Popuplock = false;

			ImGui::Indent();
			for (size_t i = 0; i < Settings.KeyBinds.size(); i++)
			{
				auto& data = UserSettings::KeyBindDataList[i];
				auto& key = Settings.KeyBinds[i];

				ImGuIHelper::ItemLabel(data.KeyBindName,ImGuIHelper::Left);

				DrawKeyBind(key);


				if (Popuplock == false)
				{
					if (ImGuIHelper::BeginPopupContextItem("KeyPopup"))
					{
						Popuplock = true;

						if (ImGui::MenuItem("Reset"))
						{
							key = data.Default;
						}
						ImGui::EndPopup();
					}
				}
			}
			ImGui::Unindent();
		}	
		
		if (ImGui::BeginTabItem("CodeEditor"))
		{
			ImGuIHelper::InputPath("CodeEditor", Settings.CodeEditorPath, {}, ImGuIHelper::InputPathType::File, Span<String>(&RunableFilesExt[0], RunableFilesExtCount));


			ImGuiContext& g = *GImGui;
			auto Size = g.FontSize + g.Style.FramePadding.y * 2;
			ImVec2 minbuttionsize = { Size,Size };

			
			ImGuIHelper::ItemLabel(StringView("Arguments to Pass"), ImGuIHelper::ItemLabelFlag::Left);

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - (minbuttionsize.x + ImGui::GetStyle().FramePadding.y * 2));
			ImGuIHelper::InputText(Settings.OpenCodeEditorFileArg);
			ImGui::PopItemWidth();

			if (IsShowingExampleArguments == false)
			{
				ImGui::SameLine();

				ImGui::PushID(4);
				bool v = ImGui::Button("+", minbuttionsize);
				ImGui::PopID();
				if (v)
				{
					IsShowingExampleArguments = true;
				}
			}
			else
			{
				ImGui::SameLine();
				if (ImGui::Button("X",minbuttionsize))
				{
					IsShowingExampleArguments = false;
				}
				ImGui::Separator();

				for (size_t i = 0; i < ArgumentExamplesCount; i++)
				{
					auto& Item = ArgumentExamples[i];

					ImGui::PushID(&Item);

					bool B = ImGui::Button("Select"); ImGui::SameLine();

					ImGuIHelper::Text(StringView(Item.EditorName)); ImGui::SameLine();

					ImGui::BeginDisabled(true);
					ImGuIHelper::InputText("", (String&)Item.Value);
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
