#pragma once
#include "UCodeRunTime/Modules/CoreModule.hpp"
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
	Vector<Unique_ptr<UEditorAssetFileData>> Assets;
	Vector<Unique_ptr<UEditorComponentData>> Components;
};
EditorEnd
