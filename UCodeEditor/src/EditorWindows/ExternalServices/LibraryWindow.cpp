#include "LibraryWindow.hpp"
#include "Helper/ImGuIHelper.hpp"
EditorStart
void LibraryWindow::UpdateWindow()
{
	thread_local bool Test = false;
	if (Test == false)
	{
		_TestMap["Hello"] = 3;
		_TestMap["World"] = 5;
		Test = true;

		_Variant = 0;
	}
	ImGuIHelper::Draw_Dictionary("TestMap", _TestMap);
	ImGuIHelper::DrawVector("TestList", _TestList);
	ImGuIHelper::EnumVariantField("Variants", _Variant);
}

EditorWindow* LibraryWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new LibraryWindow(windowdata);
}

EditorWindowData LibraryWindow::GetEditorData()
{
	return EditorWindowData("LibraryWindow", MakeWin);
}
EditorEnd
