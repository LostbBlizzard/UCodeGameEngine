#pragma once
#include "Typedefs.hpp"
#include "Plugin/UPlugin.hpp"
#include "UCodeRunTime/ULibrarys/InputManger/InputManger.hpp"
EditorStart

using KeyBindList_t = i8;
enum class KeyBindList : KeyBindList_t
{
	Special,
	Alternative,
	Extra,

	Up,
	Down,
	Left,
	Right,

	MenuBar,

	FilesWindow,
	EditorWindow,
	HierarchyWindow,
	GameWindow,
	InspectorWindow,
	ProjectWindow,
	UserWindow,
	ConsoleWindow,
	ExportWindow,
	
	Max,
};
struct KeyBinding
{
	UCode::InputKey key = UCode::InputKey::Escape;
	KeyBinding()
	{

	}

	KeyBinding(UCode::InputKey key)
	{
		this->key = key;
	}

	String ToString() const;
	static Optional<KeyBinding> Parse(StringView str);
};

struct KeyBindData
{
	StringView KeyBindName;
	KeyBinding Default;
	KeyBindData(StringView KeyBindName,KeyBinding Default)
	{
		this->KeyBindName = KeyBindName;
		this->Default = Default;
	}
};


class UserSettings
{
public:
	String CodeEditorPath;
	String OpenCodeEditorFileArg;
	bool allowautoudate = true;
	bool DeleteGoesToTrash = true;
	inline static bool updateonclose = false;
	Vector<UPluginID> GloballyActivePlugins;
	Array<KeyBinding, (KeyBindList_t)KeyBindList::Max> KeyBinds;

	static inline const char* FilePathArg = "{FilePath}";
	static inline const char* FileLineNumberArg = "{LineNumber}";
	static inline const char* ProjectPathArg = "{ProjectDir}";
	static inline const char* ColumnArg = "{Column} ";
	static UserSettings& GetSettings();
	static void SaveSettings();

	bool IsKeybindActive(KeyBindList key)
	{
		return  IsKeybindActive(KeyBinds[(size_t)key]);
	}
	bool IsKeybindActive(KeyBinding& key);	

	static bool ToFile(const Path& path,const UserSettings& Value);
	static bool FromFile(const Path& path, UserSettings& Path);

	static Path GetPath();

	static String GetOpenFileStringAsArgs(const UserSettings& Data, const Path& FileToOpen, const Path& ProjectDir, size_t LineNumber, size_t Column);

	static inline const char* FileName = "UserSettings.Yaml";

	const static Array<KeyBindData, (KeyBindList_t)KeyBindList::Max> KeyBindDataList;
};
EditorEnd