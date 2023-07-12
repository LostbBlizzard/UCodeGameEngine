#pragma once

#include <EditorWindows/EditorWindow.hpp>

#include <Editor/EditorNamespace.hpp>
EditorStart

class UserSettingsWindow :public EditorWindow
{
public:
	UserSettingsWindow(const NewEditorWindowData& windowdata);
	~UserSettingsWindow() override;
	void OnSaveWindow(USerializer& Input)  override;
	void UpdateWindow() override;

	static EditorWindowData GetEditorData();
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	bool IsShowingExampleArguments = false;
};


EditorEnd