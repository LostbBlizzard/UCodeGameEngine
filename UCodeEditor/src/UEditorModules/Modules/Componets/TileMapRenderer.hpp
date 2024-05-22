#pragma once
#include "../CoreModule.hpp"
EditorStart

class TileMapRenderUEditorData : public UEditorComponentData
{
public:
	TileMapRenderUEditorData();

	void DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const override;

	void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const override;
};

EditorEnd

