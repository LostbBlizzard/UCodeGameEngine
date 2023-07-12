#pragma once
#include "EditorWindows/EditorWindow.hpp"

EditorStart
class GitWindow : EditorWindow
{
public:
	GitWindow(const NewEditorWindowData& project):EditorWindow(project) {}
	~GitWindow() {}

	void UpdateWindow();



	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	static EditorWindowData GetEditorData();
private:
};
EditorEnd

