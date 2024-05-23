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

	static EditorWindowData GetEditorData();
private:
	static EditorWindow* MakeWin(const NewEditorWindowData& windowdata);
	TilePalettePtr _CurrentTilePalette;
};

EditorEnd 
