#pragma once

#include <EditorWindows/EditorWindow.hpp>
#include <Editor/EditorNamespace.hpp>
#include "Helper/AppFiles.hpp"
#include "BuildSytems/BuildSytemManger.hpp"
#include "UCodeRunTime/CoreSystems/Threads.hpp"
EditorStart

class ExportProjectWindow :public EditorWindow
{
public:
	enum class Platforms :u16
	{
		UCodeEditor,
		Windows,
		Mac,
		Linux_Standalone,

		Android,
		iOS,

		WebGL,

		PS5,
		Xbox_One,
		Nintendo_Switch,

		Dedicated_Server,
		
		Custom,

#if UCodeGEWindows
	ThisPlatform = Windows,
#elif UCodeGELinux
	ThisPlatform = Linux_Standalone,
#elif UCodeGEMacOS
	ThisPlatform = Mac,
#endif 
	};
	struct PlatformsData
	{
		const char* Name;
		Platforms Value;
		AppFiles::sprite Sprite;
		const char* Msg;
	};


	ExportProjectWindow(const NewEditorWindowData& windowdata);
	~ExportProjectWindow() override;
	void UpdateWindow() override;

	static EditorWindowData GetEditorData();
	static const PlatformsData* Get_PlatfromData(Platforms Value);
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);

	void ShowWindowsExportSeting();
	void ShowLinuxExportSeting();
	void ShowMacOsExportSeting();
	void ShowIosExportSeting();

	void ShowWebExportSeting();
	void SetBuildData(RunTimeProjectData* ProjectInfo,BuildSetings::SettingsType Info);
	void ShowAndroidExportSeting();
	void ShowSeverExportSeting();
	void ShowUCodeEditorExportSeting();
	void ShowCustomExportSeting();

	const PlatformsData* LookingAtPlatform = nullptr;
	BuildSytemManger buildSytem;

	WindowsBuildSetings WinSettings;
	LinuxBuildSetings linuxSettings;
	MacOsBuildSetings macSettings;

	WebBuildSetings webSettings;
	AndroidBuildSetings androidSettings;
	IOSBuildSetings iosBuildSetings;

	CustomBuildSetings customBuildSetings;
	UCode::AsynTask _Task;
};

EditorEnd