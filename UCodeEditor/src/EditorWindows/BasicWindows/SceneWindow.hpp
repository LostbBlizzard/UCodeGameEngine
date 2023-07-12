#pragma once
#include <UCodeRunTime/CoreRunTime.hpp>
#include <EditorWindows/EditorWindow.hpp>
EditorStart
class SceneWindow :public EditorWindow
{
public:
	SceneWindow(const NewEditorWindowData& project);
	~SceneWindow();

	void UpdateWindow();

	static EditorWindowData GetEditorData();
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
};
EditorEnd

