#pragma once
#include "APINameSpace.hpp"
#include "UCodeLang/RunTime/ReflectionStl.hpp"
#include "../../Typedefs.hpp"
UCodeAPIEditorStart

UCodeLangExportSymbol("UCodeGameEngineEditor") UCodeLangEmbed(
	R"(
    $Exporter trait export:
	  uintptr _Handle = 0;
    ")");


struct ExportContext
{
	const UCodeLang::ULangPathView AssetPath();

	const UCodeLang::ULangPathView OutputPath();

};


UCodeAPIEditorEnd