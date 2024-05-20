#pragma once
#include "../CoreModule.hpp"

EditorStart
class ULangScriptUEditorData : public UEditorComponentData
{
public:
	ULangScriptUEditorData();


	void DrawAsTree(const UEditorComponentDrawTreeData& Data, UCode::Compoent* Value) const override;

	void DrawInspect(const UEditorComponentDrawData& Data, UCode::Compoent* Value) const override;

};
EditorEnd
