#pragma once
#include "..\..\..\..\UCode\src\UCodeRunTime\Modules\CoreModule.hpp"
#include "../UEditorModule.hpp"


EditorStart
class CoreModule :public UEditorModule
{
public:
	CoreModule();
	~CoreModule() override;
	void Init();

	const Span<UEditorAssetFileData*> GetAssetData() override
	{
		return { (UEditorAssetFileData**)Assets.data(),Assets.size() };
	}
	const Span<UEditorComponentData*> GetComponentsData() override
	{
		return { (UEditorComponentData**)Components.data(),Components.size() };
	}
private:
	Array<Unique_ptr<UEditorAssetFileData>,3> Assets;
	Array<Unique_ptr<UEditorComponentData>, 2> Components;
};
EditorEnd
