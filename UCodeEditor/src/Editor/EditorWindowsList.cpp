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
EditorStart

EditorWindowData Data2[] =
{
	 OpenProjectWindow::GetEditorData(),
 	 ConsoleWindow::GetEditorData(),
	 ProjectFilesWindow::GetEditorData(),
	 InspectWindow::GetEditorData(),
	 GameEditorWindow::GetEditorData(),
	 ProjectWindow::GetEditorData(),
	 ExportProjectWindow::GetEditorData(),

	

	 ShaderGraphWindow::GetEditorData(),

	 UCodeGraph::GetEditorData(), 
	 GitWindow::GetEditorData(),
	 LibraryWindow::GetEditorData(),
	 UserSettingsWindow::GetEditorData(),
};
const EditorWindowData* EditorWindowsList::EditorWindows = Data2;
const size_t EditorWindowsList::EditorWindows_Size = sizeof(Data2) / sizeof(EditorWindowData);


EditorEnd