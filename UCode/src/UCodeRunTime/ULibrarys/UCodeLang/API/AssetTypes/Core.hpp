#pragma once
#include "UCodeLang/LangCore/LangDef.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
#include "../../LangNamespace.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/SceneData.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/RawEntityData.hpp"
LangStart 

UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $SceneAsset export;
    ")");

UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $EntityAsset export;
    ")");

LangEnd
