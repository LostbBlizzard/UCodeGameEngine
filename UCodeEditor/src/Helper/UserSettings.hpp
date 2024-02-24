#pragma once
#include "Typedefs.hpp"
#include "Plugin/UPlugin.hpp"
EditorStart
class UserSettings
{
public:
	String CodeEditorPath;
	String OpenCodeEditorFileArg;
	bool allowautoudate = true;
	inline static bool updateonclose = false;
	Vector<UPluginID> GloballyActivePlugins;

	static inline const char* FilePathArg = "{FilePath}";
	static inline const char* FileLineNumberArg = "{LineNumber}";
	static inline const char* ProjectPathArg = "{ProjectDir}";
	static inline const char* ColumnArg = "{Column} ";
	static UserSettings& GetSettings();
	static void SaveSettings();

	static bool ToFile(const Path& path,const UserSettings& Value);
	static bool FromFile(const Path& path, UserSettings& Path);


	static Path GetPath();

	static String GetOpenFileStringAsArgs(const UserSettings& Data, const Path& FileToOpen, const Path& ProjectDir, size_t LineNumber, size_t Column);

	static inline const char* FileName = "UserSettings.Yaml";
};
EditorEnd