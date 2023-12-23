#include "../LangNamespace.hpp"

#include "UCodeLang/LangCore/LangDef.hpp"
LangStart

UCodeLangExportSymbol("UCodeGameEngine") struct Time
{
	UCodeLangExport static float DeltaTime();
};

LangEnd