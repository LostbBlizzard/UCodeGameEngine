#include "GitWindow.hpp"
EditorStart
void GitWindow::UpdateWindow()
{

}
EditorWindow* GitWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new GitWindow(windowdata);
}
EditorWindowData GitWindow::GetEditorData()
{
	return EditorWindowData("GitWindow", MakeWin);
}
EditorEnd