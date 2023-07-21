#include "DebugingWindow.hpp"
EditorStart
DebugingWindow::DebugingWindow(const NewEditorWindowData& windowdata)
	: EditorWindow(windowdata)
{

}

DebugingWindow::~DebugingWindow()
{

}

void DebugingWindow::UpdateWindow()
{

}

EditorWindowData DebugingWindow::GetEditorData()
{
	return EditorWindowData("DebugingWindow", MakeWin);
}

EditorWindow* DebugingWindow::MakeWin(const NewEditorWindowData& windowdata) { return new DebugingWindow(windowdata); }

EditorEnd