#include "TilemapWindow.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/TileMapRenderer2d.hpp"
#include "Helper/ImGuIHelper.hpp"
#include "EditorWindows/BasicWindows/GameEditorWindow.hpp"
#include "Helper/ImGuIHelper_Asset.hpp"
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
	}

	ImGui::BeginDisabled(currenttilemap == nullptr);
	{
		ImGuIHelper_Asset::AsssetField("Palette", _CurrentTilePalette);
	}
	ImGui::EndDisabled();
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

