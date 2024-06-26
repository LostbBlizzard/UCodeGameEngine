#pragma once

#include <EditorWindows/EditorWindow.hpp>

#include <Editor/EditorNamespace.hpp>
#include "UEditorModules/Modules/Assets/Tilemap/TilePalette.hpp"
EditorStart

class TilemapWindow :public EditorWindow
{
public:
	TilemapWindow(const NewEditorWindowData& windowdata);
	~TilemapWindow() override;
	void UpdateWindow() override;

	void OnSaveWindow(USerializer& Input);
	void OnLoadWindow(UDeserializer& JsonToOutof);

	static EditorWindowData GetEditorData();
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	enum class ToolBar
	{
		Selection,
		Draw,
		Erase,
		Move,
		Copy,
	};
	TilePalettePtr _CurrentTilePalette;
	TileDataPtr _CurrentTile;
	ToolBar _toolbar = ToolBar::Selection;
};

EditorEnd 
