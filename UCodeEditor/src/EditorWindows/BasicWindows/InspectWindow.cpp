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
EditorEnd


