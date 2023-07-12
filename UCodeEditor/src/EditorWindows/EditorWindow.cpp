#include "EditorWindow.hpp"
#include "ImGui.hpp"
#include "Imgui/imgui_internal.h"
#include "Editor/EditorApp.hpp"
EditorStart
void EditorWindow::WindowStart()
{

	if (CallBegin) 
	{
		const std::string& WindowName = Get_ImGuiName();
		ImGui::Begin(WindowName.c_str(), &IsOpen, flags);
	}
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (Init == false) {
		Init = true;

	}




	_Pos = *(UCode::Vec2*)&ImGui::GetWindowPos();
	_Size = *(UCode::Vec2*)&ImGui::GetContentRegionAvail();
	_Collapsed = ImGui::IsWindowCollapsed();
	auto V = window->DockNode;

	if (IsOpen == false)
	{
		Closed = true;
	}
}
void EditorWindow::WindowEnd()
{
	if (CallBegin) 
	{
		ImGui::End();
	}
}
UCode::Gamelibrary* EditorWindow::Get_GameLib() { return _WindowData._App->Get_EditorLibrary(); }
UCode::Ref<UCode::Gamelibrary> EditorWindow::Get_GameLibRef()
{
	return  _WindowData._App->Get_EditorLibraryRef();
}
RunTimeProjectData* EditorWindow::Get_ProjectData()
{
	return _WindowData._App->Get_RunTimeProjectData();
}
EditorEnd