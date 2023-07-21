#pragma once

#include <EditorWindows/EditorWindow.hpp>
#include <Editor/EditorNamespace.hpp>
EditorStart
class DebugingWindow :public EditorWindow
{
public:
	DebugingWindow(const NewEditorWindowData& windowdata);
	~DebugingWindow() override;
	void UpdateWindow() override;

	static EditorWindowData GetEditorData();
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
};
EditorEnd
