#pragma once
#include "APITypeDefs.hpp"
#include "UCodeLang/UCodeAnalyzer/Preprocessors/CppHelper.hpp"

UCodeAPIEditorStart





UCodeLangExportSymbol("UCodeGameEngine") struct AssetPath
{
public:
	UCodeLangExport uint64_t ID()
	{
		return 0;
	}
private:
};

UCodeLangExportSymbol("UCodeGameEngine") struct OpenedProject
{
	UCodeLangExport static Path GetProjectDirPath();
};

UCodeAPIEditorEnd