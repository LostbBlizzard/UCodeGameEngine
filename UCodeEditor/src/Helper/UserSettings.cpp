#include "UserSettings.hpp"
#include "Serialization.hpp"
#include "UCodeRunTime/CoreSystems/GameFiles.hpp"
#include "FileHelper.hpp"
#include "StringHelper.hpp"
#include "UCodeRunTime/Rendering/InputHelper.hpp"
EditorStart


using InputKey = UCode::InputKey;
using Ctrl = KeyBinding::Ctrl;
using Alt = KeyBinding::Alt;
const Array<KeyBindData, (KeyBindList_t)KeyBindList::Max>  UserSettings::KeyBindDataList
{
	KeyBindData(StringView("Special"),KeyBinding(InputKey::Space)),
	KeyBindData(StringView("Alternative"),KeyBinding(InputKey::Comma)),
	KeyBindData(StringView("Extra"),KeyBinding(InputKey::Dot)),

	//Vim movement
	KeyBindData(StringView("Up"),KeyBinding(InputKey::UpArrow)),
	KeyBindData(StringView("Down"),KeyBinding(InputKey::DownArrow)),
	KeyBindData(StringView("Left"),KeyBinding(InputKey::LeftArrow)),
	KeyBindData(StringView("Right"),KeyBinding(InputKey::RightArrow)),

	//Windows
	KeyBindData(StringView("MenuBar"),KeyBinding(InputKey::M)),

	KeyBindData(StringView("FilesWindow"),KeyBinding(InputKey::F)),
	KeyBindData(StringView("EditorWindow"),KeyBinding(InputKey::E)),
	KeyBindData(StringView("HierarchyWindow"),KeyBinding(InputKey::H)),
	KeyBindData(StringView("GameWindow"),KeyBinding(InputKey::G)),
	KeyBindData(StringView("InspectorWindow"),KeyBinding(InputKey::I)),
	KeyBindData(StringView("Inspector2Window"),KeyBinding(InputKey::K)),
	KeyBindData(StringView("ProjectWindow"),KeyBinding(InputKey::P)),
	KeyBindData(StringView("UserWindow"),KeyBinding(InputKey::U)),
	KeyBindData(StringView("ConsoleWindow"),KeyBinding(InputKey::C)),
	KeyBindData(StringView("ExportWindow"),KeyBinding(InputKey::O)),

	//Context Bases
	KeyBindData(StringView("New"),KeyBinding(InputKey::N,Ctrl())),
	KeyBindData(StringView("Delete"),KeyBinding(InputKey::D,Ctrl())),
	KeyBindData(StringView("Rename"),KeyBinding(InputKey::R,Ctrl())),
	KeyBindData(StringView("Inspect"),KeyBinding(InputKey::I,Ctrl())),
	KeyBindData(StringView("Copy"),KeyBinding(InputKey::C,Ctrl())),
	KeyBindData(StringView("Paste"),KeyBinding(InputKey::V,Ctrl())),
	KeyBindData(StringView("Undo"),KeyBinding(InputKey::Z,Ctrl())),
	KeyBindData(StringView("Redo"),KeyBinding(InputKey::Y,Ctrl())),
	KeyBindData(StringView("Search"),KeyBinding(InputKey::F,Ctrl())),
};
const Array<KeyBinding, (KeyBindList_t)KeyBindList::Max>  KeyBindVimMode
{
	KeyBinding(InputKey::Space),
	KeyBinding(InputKey::Comma),
	KeyBinding(InputKey::Dot),

	
	KeyBinding(InputKey::K),
	KeyBinding(InputKey::J),
	KeyBinding(InputKey::H),
	KeyBinding(InputKey::L),

	
	KeyBinding(InputKey::M),

	KeyBinding(InputKey::F),
	KeyBinding(InputKey::E),
	KeyBinding(InputKey::H),
	KeyBinding(InputKey::G),
	KeyBinding(InputKey::I),
	KeyBinding(InputKey::K),
	KeyBinding(InputKey::P),
	KeyBinding(InputKey::U),
	KeyBinding(InputKey::C),
	KeyBinding(InputKey::O),

	
	KeyBinding(InputKey::N),
	KeyBinding(InputKey::D),
	KeyBinding(InputKey::R),
	KeyBinding(InputKey::I),
	KeyBinding(InputKey::Y),
	KeyBinding(InputKey::P),
	KeyBinding(InputKey::U),
	KeyBinding(InputKey::R),
	KeyBinding(InputKey::ForwardSlash),

};
static bool HasLoadSettings = false;
static UserSettings UCodeEditor_UserSettings = {};
UserSettings& UserSettings::GetSettings()
{
	if (!HasLoadSettings)
	{

		bool Ok = FromFile(GetPath(), UCodeEditor_UserSettings);
		if (!Ok)
		{
#if UCodeGEWindows
			UCodeEditor_UserSettings.CodeEditorPath = "C:/Windows/System32/notepad.exe";
			UCodeEditor_UserSettings.OpenCodeEditorFileArg = "/W " + (String)UserSettings::FilePathArg;//open file as unicode
#else
			UCodeGEThrow("default code editor was not given  for the platform");
#endif // 

			UCodeEditor_UserSettings.ResetKeyBindsToDefault();
			ToFile(GetPath(), UCodeEditor_UserSettings);
		}

		HasLoadSettings = true;
	}
	return  UCodeEditor_UserSettings;
}
void UserSettings::SaveSettings()
{
	ToFile(GetPath(), UCodeEditor_UserSettings);
}

String KeyBinding::ToString() const
{
	String r;
	if (IsCtrl)
	{
		r += "Ctrl+";
	}
	if (IsShift)
	{	
		r += "Shift+";
	}
	if (IsAlt)
	{	
		r += "Alt+";
	}
	r += UCode::RenderAPI::UCodeKeyToString(key);
	return r;
}
Optional<KeyBinding> KeyBinding::Parse(StringView str)
{
	KeyBinding r;

	if (StringHelper::StartsWith(str, "Ctrl+"))
	{
		r.IsCtrl = true;
		str = str.substr(sizeof("Ctrl+") - 1);
	}

	if (StringHelper::StartsWith(str, "Shift+"))
	{
		r.IsShift = true;
		str = str.substr(sizeof("Shift+") - 1);
	}

	if (StringHelper::StartsWith(str, "Alt+"))
	{
		r.IsAlt = true;
		str = str.substr(sizeof("Alt+") - 1);
	}

	r.key = UCode::RenderAPI::ToStringToKey(str);
	return r;
}
bool UserSettings::ToFile(const Path& path, const UserSettings& Value)
{
	USerializer Output(USerializerType::Readable);
	Output.Write("CodeEditorPath", Value.CodeEditorPath);
	Output.Write("OpenCodeEditorFileArg", Value.OpenCodeEditorFileArg);
	Output.Write("AutoUpdate", Value.allowautoudate);
	Output.Write("DeleteGoesToTrash", Value.DeleteGoesToTrash);
	Output.Write("GlobalPlugins", Value.GloballyActivePlugins);

	auto& text = Output.Get_TextMaker();

	{
		text << YAML::Key << "KeyBinds" << YAML::Value;

		text << YAML::BeginSeq;

		for (size_t i = 0; i < (KeyBindList_t)KeyBindList::Max; i++)
		{
			auto& KeyData = KeyBindDataList[i];
			auto& UserKey = Value.KeyBinds[i];

			YAML::Node v;
			v.push_back(String(KeyData.KeyBindName));
			v.push_back(UserKey.ToString());

			text << v;
		}

		text << YAML::EndSeq;
	}

	return Output.ToFile(path, false);
}

void UserSettings::ResetKeyBindsToDefault()
{
	for (size_t i = 0; i < (KeyBindList_t)KeyBindList::Max; i++)
	{
		auto& KeyData = KeyBindDataList[i];
		auto& UserKey = KeyBinds[i];

		UserKey = KeyData.Default;
	}

}
void UserSettings::SetAsVimKeyBinds()
{
	for (size_t i = 0; i < (KeyBindList_t)KeyBindList::Max; i++)
	{
		auto& KeyData = KeyBindVimMode[i];
		auto& UserKey = KeyBinds[i];

		UserKey = KeyData;
	}

}
bool UserSettings::FromFile(const Path& path, UserSettings& Value)
{
	String Text;
	String line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			Text += line + '\n';
		}
		myfile.close();



		UDeserializer Input; Input.SetYAMLString(Text);

		auto& yaml = Input.Get_TextReader();
		
		Value.ResetKeyBindsToDefault();

		Input.ReadType("CodeEditorPath", Value.CodeEditorPath, Value.CodeEditorPath);
		Input.ReadType("OpenCodeEditorFileArg", Value.OpenCodeEditorFileArg, Value.OpenCodeEditorFileArg);

		if (yaml["AutoUpdate"]) {
			Input.ReadType("AutoUpdate", Value.allowautoudate);
		}
		if (yaml["DeleteGoesToTrash"]) {
			Input.ReadType("DeleteGoesToTrash", Value.DeleteGoesToTrash);
		}
		

		if (yaml["KeyBinds"])
		{
			auto list = yaml["KeyBinds"];

			for (auto Item : list)
			{
				auto keybind = Item[0].as<String>();
				auto key = Item[1].as<String>();

				for (size_t i = 0; i < KeyBindDataList.size(); i++)
				{
					auto& Item = KeyBindDataList[i];

					if (Item.KeyBindName == keybind)
					{
						auto p = KeyBinding::Parse(key);

						if (p.has_value())
						{
							Value.KeyBinds[i] = p.value();
							break;
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}

Path UserSettings::GetPath()
{
	return FileHelper::Get_PersistentDataPath() / Path(FileName).native();
}
String UserSettings::GetOpenFileStringAsArgs(const UserSettings& Data, const Path& FileToOpen, const Path& ProjectDir, size_t LineNumber, size_t Column)
{
	String V = Data.OpenCodeEditorFileArg;

	StringHelper::Replace(V, UserSettings::FilePathArg, FileToOpen.generic_string());
	StringHelper::Replace(V, UserSettings::FileLineNumberArg, std::to_string(LineNumber));
	StringHelper::Replace(V, UserSettings::ProjectPathArg, ProjectDir.generic_string());
	StringHelper::Replace(V, UserSettings::ColumnArg, std::to_string(Column));
	return V;
}
bool UserSettings::IsKeybindActive(KeyBinding& key)
{
	if (key.key == UCode::InputKey::Null) { return false; }

	auto imkey = UCode::RenderAPI::UCodeToImguiKey(key.key);

	if (key.IsAlt) 
	{

		if (!(ImGui::IsKeyDown(ImGuiMod_Alt)))
		{
			return false;
		}
	
	}
	if (key.IsCtrl) 
	{

		if (!(ImGui::IsKeyDown(ImGuiMod_Ctrl)))
		{
			return false;
		}
	
	}
	if (key.IsShift) 
	{

		if (!(ImGui::IsKeyDown(ImGuiMod_Shift)))
		{
			return false;
		}
	
	}
	return ImGui::IsKeyPressed(imkey);
}
void UserSettings::SetCopyBuffer(const String& str)
{
	ImGui::SetClipboardText(str.c_str());
}
String UserSettings::GetCopyBuffer()
{
	auto s = ImGui::GetClipboardText();
	if (s)
	{
		return s;
	}
	return {};
}

static AnyManagedPtr ref;
void UserSettings::SetCopyManagedRef(AnyManagedPtr ptr)
{
	ref = ptr;
}
UC::AnyManagedPtr UserSettings::GetCopyedManagedRef()
{
	return ref;
}
EditorEnd

