#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include "UCodeRunTime/ULibrarys/UCodeLang/ScirptableObject.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/UCodeLangAssets.hpp"
EditorStart
class ScriptableAssetFile :public UEditorAssetFileData
{
public:
	ScriptableAssetFile();

	class Liveing :public UEditorAssetFile
	{
	public:
		UC::ScirptableObjectAsset _Object;

		UC::ScirptableObjectData _Data;
		UC::ScirptableObjectData _ReloadBufferObject;
		UC::ScirptableObjectData _InFile;

		Optional<UC::ULangRunTime::ScriptInfoKey> _ScriptKey = {};
		bool justsaved = false;
		Liveing();
		~Liveing();

		void Init(const UEditorAssetFileInitContext& Context) override;
		virtual void FileUpdated();
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override;
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override;
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;
		void TryLoadAsset();
		NullablePtr<UCode::Asset> LoadAsset(const LoadAssetContext& Item) override;
	};

	ExportFileRet ExportFile(const Path& path, const ExportFileContext& Item) override;

	Optional<GetUIDInfo> GetFileUID(UEditorGetUIDContext& context) override;

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr<UEditorAssetFile>(new Liveing());
	}

};
EditorEnd

