#pragma once


#include "../UEditorModule.hpp"


EditorStart
class CodeModule :public UEditorModule
{
public:
	CodeModule();
	~CodeModule() override;
	void Init();


	const Span<UEditorAssetFileData*> GetAssetData() override
	{
		return { (UEditorAssetFileData**)Assets.data(),Assets .size()};
	}

	const Span<UEditorComponentData*> GetComponentsData() override
	{
		return { (UEditorComponentData**)Components.data(),Components.size() };
	}


	void FilesUpdated(const Vector<FileUpdateData>& paths) override;

private:
	Array<Unique_ptr<UEditorAssetFileData>,2> Assets;
	Array<Unique_ptr<UEditorComponentData>,1> Components;
};

EditorEnd
