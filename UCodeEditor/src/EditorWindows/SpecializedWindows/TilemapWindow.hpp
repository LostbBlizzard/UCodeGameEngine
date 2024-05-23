#pragma once

#include <EditorWindows/EditorWindow.hpp>

#include <Editor/EditorNamespace.hpp>
EditorStart


class TilemapWindow :public EditorWindow
{
public:
	TilemapWindow(const NewEditorWindowData& windowdata);
	~TilemapWindow() override;
	void UpdateWindow() override;

	static EditorWindowData GetEditorData();
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
};

EditorEnd 
