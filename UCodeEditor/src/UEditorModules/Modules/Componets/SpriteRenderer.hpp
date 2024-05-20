#pragma once
#include "../CoreModule.hpp"
EditorStart

class SpriteRendererUEditorData : public UEditorComponentData
{
public:
	SpriteRendererUEditorData();

	void DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const override;

	void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const override;
};

EditorEnd
