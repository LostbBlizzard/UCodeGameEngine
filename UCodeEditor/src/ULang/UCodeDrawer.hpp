#pragma once
#include "UCodeRunTime/BasicTypes.hpp"
#include <Editor/EditorNamespace.hpp>
#include <UCodeLang/UCodeLang/LangCore/UClib.hpp>
#include <Helper/ImGuIHelper.hpp>
EditorStart
class UCodeDrawer
{
public:

	static bool DrawType(void* Pointer, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly,bool IfClassRemoveFlags = false);

	static bool DrawClass(void* Pointer, const UCodeLang::Class_Data& Class, const UCodeLang::ClassAssembly& Assembly);

	static bool DrawField(void* Pointer, const char* FieldName, const UCodeLang::ReflectionTypeInfo& Type, const UCodeLang::ClassAssembly& Assembly);

	static bool DrawEnum(void* Pointer, const UCodeLang::Enum_Data& Class, const UCodeLang::ClassAssembly& Assembly);
};
EditorEnd
