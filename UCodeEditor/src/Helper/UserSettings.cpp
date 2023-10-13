#include "UserSettings.hpp"
#include "Serialization.hpp"
#include "UCodeRunTime/CoreBooks/GameFiles.hpp"
#include "FileHelper.hpp"
#include "StringHelper.hpp"
EditorStart



static bool HasLoadSettings = false;
static UserSettings UCodeEditor_UserSettings = {};
UserSettings& UserSettings::GetSettings()
{
	if (!HasLoadSettings)
	{

		bool Ok = FromFile(GetPath(), UCodeEditor_UserSettings);
		if (!Ok)
		{
			#if UCodeGameEnginePlatformWindows
			UCodeEditor_UserSettings.CodeEditorPath = "C:/Windows/System32/notepad.exe";
			UCodeEditor_UserSettings.OpenCodeEditorFileArg = "/W " + (String)UserSettings::FilePathArg;//open file as unicode
			#else
			UCodeGameEngineThrowException("default code editor was not given  for the platform");
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

bool UserSettings::ToFile(const Path& path,const UserSettings& Value)
{
	USerializer Output(USerializerType::Readable);
	Output.Write("CodeEditorPath",Value.CodeEditorPath);
	Output.Write("OpenCodeEditorFileArg", Value.OpenCodeEditorFileArg);

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
		Input.ReadType("CodeEditorPath", Value.CodeEditorPath, Value.CodeEditorPath);
		Input.ReadType("OpenCodeEditorFileArg", Value.OpenCodeEditorFileArg, Value.OpenCodeEditorFileArg);
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


EditorEnd

