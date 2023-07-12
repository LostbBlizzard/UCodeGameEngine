#pragma once



#include <EditorWindows/EditorWindow.hpp>

#include <Editor/EditorNamespace.hpp>
EditorStart


class ProjectWindow :public EditorWindow
{
public:
	ProjectWindow(const NewEditorWindowData& windowdata);
	~ProjectWindow() override;
	void SaveProjectDatatFile();
	void UpdateWindow() override;

	static EditorWindowData GetEditorData();
private:
	String NameBuffer;
	float SaveCountDown = 0;
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
};

EditorEnd 