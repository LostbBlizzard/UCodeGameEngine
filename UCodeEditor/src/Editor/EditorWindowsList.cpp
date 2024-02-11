#include "EditorWindowsList.hpp"

#include <EditorWindows/BasicWindows/GameEditorWindow.hpp>
#include <EditorWindows/BasicWindows/ConsoleWindow.hpp>
#include <EditorWindows/BasicWindows/InspectWindow.hpp>
#include <EditorWindows/ProjectManagement/ProjectFilesWindow.hpp>
#include <EditorWindows/ProjectManagement/ProjectWindow.hpp>
#include <EditorWindows/ProjectManagement/OpenProjectWindow.hpp>
#include <EditorWindows/ProjectManagement/ExportProjectWindow.hpp>
#include <EditorWindows/GraphWindows/ShaderGraphWindow.hpp>
#include <EditorWindows/GraphWindows/UCodeGraph.hpp>
#include <EditorWindows/Git/GitWindow.hpp>
#include <EditorWindows/ExternalServices/LibraryWindow.hpp>

#include <EditorWindows/BasicWindows/UserSettingsWindow.hpp>
#include <EditorWindows/BasicWindows/DebugingWindow.hpp>
EditorStart

Variant<EditorWindowData, EditorWindowsList::NewMenu, EditorWindowsList::EndMenu> Data2[] =
{
	//OpenProjectWindow  Must be first Item because 
	//EditorAppCompoent will load this  Data2[0] when app starts 

	EditorWindowsList::NewMenu("Other"),

	OpenProjectWindow::GetEditorData(),
	UserSettingsWindow::GetEditorData(),

	EditorWindowsList::EndMenu(),

	EditorWindowsList::NewMenu("Core"),

	InspectWindow::GetEditorData(),
	GameEditorWindow::GetEditorData(),
	ProjectFilesWindow::GetEditorData(),
	ConsoleWindow::GetEditorData(),
	ProjectWindow::GetEditorData(),
	ExportProjectWindow::GetEditorData(),

	EditorWindowsList::EndMenu(),
	
	EditorWindowsList::NewMenu("Specialized"),
	
	DebugingWindow::GetEditorData(),
	ShaderGraphWindow::GetEditorData(),
	//UCodeGraph::GetEditorData(),
	GitWindow::GetEditorData(),
	LibraryWindow::GetEditorData(),
	
	EditorWindowsList::EndMenu(),
};
const Variant<EditorWindowData,EditorWindowsList::NewMenu,EditorWindowsList::EndMenu>* EditorWindowsList::EditorWindows = Data2;
const size_t EditorWindowsList::EditorWindows_Size = sizeof(Data2) / sizeof(EditorWindowData);


EditorEnd