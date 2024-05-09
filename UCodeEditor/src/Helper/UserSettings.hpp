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

	New,
	Delete,
	Rename,
	Inspect,
	Copy,
	Paste,
	Undo,
	Redo,
	Search,

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

	static void SetCopyBuffer(String& str);
	static String GetCopyBuffer();

	template<typename T>
	static Optional<T> ReadCopyBufferAs(const StringView& UType)
	{
		return ReadCopyBufferAs<T>(UType, GetCopyBuffer());
	}
	
	template<typename T>
	static Optional<T> ReadCopyBufferAs(const StringView& UType, const String& CopyBuffer)
	{
		bool isgood = false;

		YAML::Node tep;
		{
			bool ok = true;
			try
			{
				tep = YAML::Load(CopyBuffer);
			}
			catch (YAML::ParserException ex)
			{
				ok = false;
			}
			if (ok && tep.IsMap())
			{
				isgood = (bool)tep["UType"];
				if (isgood)
				{
					isgood = tep["UType"].as<String>("") == UType && (bool)tep["UData"];
				}
			}
		}

		if (isgood)
		{
			return tep["UData"].as<T>();
		}
		return {};
	}

	
	template<typename T>
	static String SetCopyBufferAsValueStr(const StringView& UType,const T& Value)
	{
		USerializer V(USerializerType::YAML);
		V.Write("UData", Value);
		V.Write("UType", String(UType));

		auto copytext = V.Get_TextMaker().c_str();

		return copytext;
	}
	template<typename T>
	static void SetCopyBufferAsValue(const StringView& UType,const T& Value)
	{	
		UserSettings::SetCopyBuffer(SetCopyBufferAsValueStr(UType,Value));
	}
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