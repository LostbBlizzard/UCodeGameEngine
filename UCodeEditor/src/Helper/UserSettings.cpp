#include "UserSettings.hpp"
#include "Serialization.hpp"
#include "UCodeRunTime/CoreSystems/GameFiles.hpp"
#include "FileHelper.hpp"
#include "StringHelper.hpp"
#include "UCodeRunTime/Rendering/InputHelper.hpp"
EditorStart


using InputKey = UCode::InputKey;
const Array<KeyBindData,(KeyBindList_t)KeyBindList::Max>  UserSettings::KeyBindDataList
{
	KeyBindData(StringView("Special"),KeyBinding(InputKey::Space)),
	KeyBindData(StringView("Alternative"),KeyBinding(InputKey::DownArrow)),	
	KeyBindData(StringView("Extra"),KeyBinding(InputKey::LeftArrow)),

	//Vim movement
	KeyBindData(StringView("Up"),KeyBinding(InputKey::K)),
	KeyBindData(StringView("Down"),KeyBinding(InputKey::J)),
	KeyBindData(StringView("Left"),KeyBinding(InputKey::H)),
	KeyBindData(StringView("Right"),KeyBinding(InputKey::L)),

	//Windows
	KeyBindData(StringView("MenuBar"),KeyBinding(InputKey::M)),

	KeyBindData(StringView("FilesWindow"),KeyBinding(InputKey::F)),	
	KeyBindData(StringView("EditorWindow"),KeyBinding(InputKey::E)),	
	KeyBindData(StringView("HierarchyWindow"),KeyBinding(InputKey::H)),	
	KeyBindData(StringView("GameWindow"),KeyBinding(InputKey::G)),	
	KeyBindData(StringView("InspectorWindow"),KeyBinding(InputKey::I)),	
	KeyBindData(StringView("ProjectWindow"),KeyBinding(InputKey::P)),	
	KeyBindData(StringView("UserWindow"),KeyBinding(InputKey::U)),		
	KeyBindData(StringView("ConsoleWindow"),KeyBinding(InputKey::C)),	
	KeyBindData(StringView("ExportWindow"),KeyBinding(InputKey::O)),

	//Context Bases
	KeyBindData(StringView("New"),KeyBinding(InputKey::N)),
	KeyBindData(StringView("Delete"),KeyBinding(InputKey::D)),
	KeyBindData(StringView("Rename"),KeyBinding(InputKey::R)),
	KeyBindData(StringView("Inspect"),KeyBinding(InputKey::I)),
	KeyBindData(StringView("Copy"),KeyBinding(InputKey::C)),
	KeyBindData(StringView("Paste"),KeyBinding(InputKey::P)),
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
	return UCode::RenderAPI::UCodeKeyToString(key);
}
Optional<KeyBinding> KeyBinding::Parse(StringView str)
{
	KeyBinding r;
	r.key = UCode::RenderAPI::ToStringToKey(str);
	return r;
}
bool UserSettings::ToFile(const Path& path,const UserSettings& Value)
{
	USerializer Output(USerializerType::Readable);
	Output.Write("CodeEditorPath",Value.CodeEditorPath);
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

		Input.ReadType("CodeEditorPath", Value.CodeEditorPath, Value.CodeEditorPath);
		Input.ReadType("OpenCodeEditorFileArg", Value.OpenCodeEditorFileArg, Value.OpenCodeEditorFileArg);
		
		if (yaml["AutoUpdate"]) {
			Input.ReadType("AutoUpdate", Value.allowautoudate);
		}
		if (yaml["DeleteGoesToTrash"]) {
			Input.ReadType("DeleteGoesToTrash", Value.DeleteGoesToTrash);
		}

		for (size_t i = 0; i < (KeyBindList_t)KeyBindList::Max; i++)
		{
			auto& KeyData = KeyBindDataList[i];
			auto& UserKey = Value.KeyBinds[i];

			UserKey = KeyData.Default;
		}

		if (yaml["KeyBinds"]) 
		{
			auto& list = yaml["KeyBinds"];
			
			for (auto& Item : list)
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
	return FileHelper::Get_PersistentDataPath().native() + Path(FileName).native();
}
String UserSettings::GetOpenFileStringAsArgs(const UserSettings& Data, const Path& FileToOpen, const Path& ProjectDir, size_t LineNumber, size_t Column)
{
	String V = Data.OpenCodeEditorFileArg;

	StringHelper::Replace(V,UserSettings::FilePathArg, FileToOpen.generic_string());
	StringHelper::Replace(V, UserSettings::FileLineNumberArg, std::to_string(LineNumber));
	StringHelper::Replace(V, UserSettings::ProjectPathArg, ProjectDir.generic_string());
	StringHelper::Replace(V, UserSettings::ColumnArg,std::to_string(Column));
	return V;
}
bool UserSettings::IsKeybindActive(KeyBinding& key)
{
    auto imkey = UCode::RenderAPI::UCodeToImguiKey(key.key);

	return ImGui::IsKeyDown(imkey);
}

EditorEnd

