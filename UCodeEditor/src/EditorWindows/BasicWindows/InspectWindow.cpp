#include "InspectWindow.hpp"
#include "ImGui.hpp"
#include <Helper/ImGuIHelper.hpp>
#include <Editor/EditorAppCompoent.hpp>
#include "Helper/ImGuIHelper_Asset.hpp"
#include "Helper/UserSettings.hpp"
EditorStart

InspectWindow::InspectWindow(const NewEditorWindowData& windowdata) : EditorWindow(windowdata), _Inspect(), _InspectDrawer(this)
{
	WindowName = "InspectWindow";
}

InspectWindow::~InspectWindow()
{
}

void InspectWindow::UpdateWindow()
{
	auto& settings = UserSettings::GetSettings();
	if (Get_App()->GetInputMode() == KeyInputMode::Window)
	{
		if (settings.IsKeybindActive(KeyBindList::InspectorWindow))
		{
			ImGui::SetWindowFocus();
			Get_App()->SetToNormal();
		}
	}
	if (_Inspect._Draw != nullptr)
	{
		if (_Updated)
		{
			_Updated = false;
			ImGui::SetWindowFocus();
		}
		_Inspect._Draw(_InspectDrawer);
	}
}
EditorWindowData InspectWindow::GetEditorData()
{
	return EditorWindowData("InspectWindow", MakeWin);
}
EditorWindow* InspectWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new InspectWindow(windowdata);
}

Inspect2Window::Inspect2Window(const NewEditorWindowData& windowdata) : EditorWindow(windowdata), _Inspect(), _InspectDrawer(this)
{

}
Inspect2Window::~Inspect2Window()
{
}
void Inspect2Window::UpdateWindow()
{
	auto& settings = UserSettings::GetSettings();
	if (Get_App()->GetInputMode() == KeyInputMode::Window)
	{
		if (settings.IsKeybindActive(KeyBindList::Inspector2Window))
		{
			ImGui::SetWindowFocus();
			Get_App()->SetToNormal();
		}
	}
	if (_Inspect._Draw != nullptr)
	{
		if (_Updated)
		{
			_Updated = false;
			ImGui::SetWindowFocus();
		}
		_Inspect._Draw(_InspectDrawer);
	}
}
EditorWindowData Inspect2Window::GetEditorData()
{
	return EditorWindowData("Inspect2Window", MakeWin);
}
EditorWindow* Inspect2Window::MakeWin(const NewEditorWindowData& windowdata)
{
	return new Inspect2Window(windowdata);
}
EditorEnd


