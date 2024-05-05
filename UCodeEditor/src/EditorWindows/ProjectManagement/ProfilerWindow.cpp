#include "ProfilerWindow.hpp"

EditorStart
ProfilerWindow::ProfilerWindow(const NewEditorWindowData& project)
	: EditorWindow(project)
{

}
ProfilerWindow::~ProfilerWindow() 
{

}
void ProfilerWindow::UpdateWindow() 
{

}
void ProfilerWindow::OnSaveWindow(USerializer& JsonToSaveIn) 
{

}
void ProfilerWindow::OnLoadWindow(UDeserializer& JsonToOutof) 
{

}
EditorWindowData ProfilerWindow::GetEditorData()
{
	return EditorWindowData("ProfilerWindow", MakeWin);
}
EditorWindow* ProfilerWindow::MakeWin(const NewEditorWindowData& project)
{
    return new ProfilerWindow(project);
}

EditorEnd

