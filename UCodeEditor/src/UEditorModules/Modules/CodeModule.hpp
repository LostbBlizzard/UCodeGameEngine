#pragma once


#include "../UEditorModule.hpp"
#include "UCodeLang/UCodeLang.hpp"

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

	static void BuildUCode(bool IsInEditor);
	static const Vector<const UCodeLang::AssemblyNode*>& GetAllVaildCompoents();


	void FilesUpdated(const Vector<FileUpdateData>& paths) override;
	Result<Vector<ExportEditorReturn>, ExportErrors> ExportSystems(const ExportEditorContext& Context) override;
private:
	Array<Unique_ptr<UEditorAssetFileData>,3> Assets;
	Array<Unique_ptr<UEditorComponentData>,1> Components;
};

EditorEnd
