#pragma once
#include "UCodeLang/LangCore/LangDef.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
#include "../../LangNamespace.hpp"
#include "UCodeRunTime/ULibrarys/Rendering/GPUTypes.hpp"
LangStart 

UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $Sprite export;
    ")");

UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $Texture export;
    ")");

LangEnd
