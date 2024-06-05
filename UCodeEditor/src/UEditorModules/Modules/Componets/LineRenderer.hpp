#pragma once
#include "../CoreModule.hpp"
EditorStart

class LineRendererUEditorData : public UEditorComponentData
{
public:
	LineRendererUEditorData();

	void DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const override;

	void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const override;
};

EditorEnd
