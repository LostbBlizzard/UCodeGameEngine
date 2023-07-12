#include "SceneWindow.hpp"
EditorStart
SceneWindow::SceneWindow(const NewEditorWindowData& project) : EditorWindow(project)
{
}
SceneWindow::~SceneWindow()
{
}
void SceneWindow::UpdateWindow()
{
}
EditorWindowData SceneWindow::GetEditorData()
{
	return EditorWindowData("SceneWindow", MakeWin);
}
EditorWindow* SceneWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new SceneWindow(windowdata);
}
EditorEnd

