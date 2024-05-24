#include "TilemapWindow.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/TileMapRenderer2d.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "EditorWindows/BasicWindows/GameEditorWindow.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"

#include "UCodeRunTime/ULibrarys/Serialization/Yaml_Implementation/AsssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/Bit_Implementation/AsssetPtr.hpp"
EditorStart
TilemapWindow::TilemapWindow(const NewEditorWindowData& windowdata):EditorWindow(windowdata)
{

}
TilemapWindow::~TilemapWindow()
{
}

void TilemapWindow::UpdateWindow()
{
	auto currententity = GameEditorWindow::GetCurrentSeclectedEntity();
	UC::TileMapRenderer* currenttilemap = nullptr;

	if (currententity.Has_Value())
	{
		currenttilemap = currententity.Get_Value()->GetCompent<UC::TileMapRenderer>().value_unchecked();
	}
	if (currenttilemap == nullptr)
	{
		ImGuIHelper::Text(StringView("Seclect a Tilemap in Scene Hierarchy"));

        ImGui::Separator();
	}

	//ImGui::BeginDisabled(currenttilemap == nullptr);
	{
		if (ImGui::RadioButton("Draw", _toolbar == ToolBar::Draw)) { _toolbar = ToolBar::Draw; }
        ImGui::SameLine();
	
		if (ImGui::RadioButton("Copy", _toolbar == ToolBar::Erase)) { _toolbar = ToolBar::Copy; }
        ImGui::SameLine();
	
		if (ImGui::RadioButton("Move", _toolbar == ToolBar::Move)) { _toolbar = ToolBar::Move; }
        ImGui::SameLine();	

		if (ImGui::RadioButton("Erase", _toolbar == ToolBar::Erase)) { _toolbar = ToolBar::Erase; }	
    
        ImGui::Separator();
	}
	//ImGui::EndDisabled();
	{
		ImGuIHelper_Asset::AsssetField("Palette", _CurrentTilePalette);
	}
}

void TilemapWindow::OnSaveWindow(USerializer& Input)
{
	Input.Write("_CurrentTilePalette", _CurrentTilePalette);
}

void TilemapWindow::OnLoadWindow(UDeserializer& JsonToOutof)
{
	JsonToOutof.ReadType("_CurrentTilePalette", _CurrentTilePalette,_CurrentTilePalette);
}

EditorWindowData TilemapWindow::GetEditorData()
{
	return EditorWindowData("TilemapWindow",MakeWin);
}

EditorWindow* TilemapWindow::MakeWin(const NewEditorWindowData& windowdata)
{
	return new TilemapWindow(windowdata);
}
EditorEnd

