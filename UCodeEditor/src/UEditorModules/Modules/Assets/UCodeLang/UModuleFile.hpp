#pragma once
#include "UEditorModules/Modules/CoreModule.hpp" 
#include "UCodeLang/Compilation/ModuleFile.hpp"
EditorStart
class UModuleFile :public UEditorAssetFileData
{
public:
	UModuleFile();

	class Liveing :public UEditorAssetFile
	{
	public:
		bool justsaved = false;
		UCodeLang::ModuleFile _Module;
		bool updated = false;

		Liveing()
		{

		}
		~Liveing()
		{

		}

		void Init(const UEditorAssetFileInitContext& Context) override;
		virtual void FileUpdated();
		void SaveFile(const UEditorAssetFileSaveFileContext& Context) override;
		bool DrawButtion(const UEditorAssetDrawButtionContext& Item) override;
		void DrawInspect(const UEditorAssetDrawInspectContext& Item) override;
	};

	virtual Unique_ptr<UEditorAssetFile> GetMakeNewAssetFile() override
	{
		return Unique_ptr<UEditorAssetFile>(new Liveing());
	}

};
EditorEnd
