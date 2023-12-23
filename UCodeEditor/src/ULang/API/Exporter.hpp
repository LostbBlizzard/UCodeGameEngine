#pragma once
#include "APINameSpace.hpp"
#include "UCodeLang/RunTime/ReflectionStl.hpp"
#include "../../Typedefs.hpp";
UCodeAPIEditorStart

UCodeLangExportSymbol("UCodeGameEngineEditor") UCodeLangEmbed(
	R"(
    $Exporter trait:
	  uintptr _Handle = 0;
    ")");

UCodeLangExportSymbol("UCodeGameEngineEditor") struct ExportContext
{
	const UCodeLang::ULangPathView AssetPath();

	const UCodeLang::ULangPathView OutputPath();

};


UCodeAPIEditorEnd