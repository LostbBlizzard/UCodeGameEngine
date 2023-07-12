#pragma once

#include "EditorWindows/EditorWindow.hpp"
#include <variant>
EditorStart
class LibraryWindow :EditorWindow
{
public:
	LibraryWindow(const NewEditorWindowData& project) :EditorWindow(project) {}
	~LibraryWindow(){}

	void UpdateWindow();




	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	static EditorWindowData GetEditorData();
private:
	Unordered_map<String, int> _TestMap;
	Vector<int> _TestList;
	Variant<int, bool, String> _Variant = 6;
};
EditorEnd

