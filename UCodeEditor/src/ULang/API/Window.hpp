#pragma once
#include "APINameSpace.hpp"
#include "UCodeLang/RunTime/ReflectionStl.hpp"
#include "../../Typedefs.hpp"
UCodeAPIEditorStart

UCodeLangExportSymbol("UCodeGameEngineEditor") UCodeLangEmbed(
	R"(
    $Window trait export:
	  uintptr _Handle = 0;
    ")");

UCodeLangExportSymbol("UCodeGameEngineEditor") UCodeLangEmbed(
	R"(
    $OverWriteTypeDraw trait export;
    ")");

UCodeLangExportSymbol("UCodeGameEngineEditor") UCodeLangEmbed(
	R"(
    $EditorUI export:
	 export |Field[imut StringSpan FieldName,uint8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,uint16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,uint32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,uint64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

	  
	 export |Field[imut StringSpan FieldName,int8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,int16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,int32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,int64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

	 export |Field[imut StringSpan FieldName,uintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);
	 export |Field[imut StringSpan FieldName,sintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);

	 //export |Field<T>[imut StringSpan FieldName,T& Value] => false;
     //Readonly

	 export |Field[imut StringSpan FieldName,imut uint8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,imut uint16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,imut uint32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,imut uint64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

	  
	 export |Field[imut StringSpan FieldName,imut int8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,imut int16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,imut int32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	 export |Field[imut StringSpan FieldName,imut int64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

	 export |Field[imut StringSpan FieldName,imut uintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);
	 export |Field[imut StringSpan FieldName,imut sintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);

	 //export |Field<T>[imut StringSpan FieldName,T& Value] => false;
    ")");


UCodeLangExportSymbol("UCodeGameEngineEditorInternal") struct WindowDrawAPI
{
	UCodeLangExport static bool Field(const StringView FieldName, uint8_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, uint16_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, uint32_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, uint64_t& Value);

	UCodeLangExport static bool Field(const StringView FieldName, int8_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, int16_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, int32_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, int64_t& Value);

	UCodeLangExport static bool Fieldptr(const StringView FieldName, uintptr_t& Value);
	UCodeLangExport static bool Fieldptr(const StringView FieldName, intptr_t& Value);

	UCodeLangExport static bool Field(const StringView FieldName, bool& Value);

	UCodeLangExport static bool Field(const StringView FieldName, float& Value);
	UCodeLangExport static bool Field(const StringView FieldName, double& Value);

	static bool Field(const StringView FieldName, void* Value, uintptr_t idtype);
	static bool FieldRawView(const StringView FieldName, void* Value, uintptr_t idtype);
	//
	UCodeLangExport static bool Field(const StringView FieldName, const uint8_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, const uint16_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, const uint32_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, const uint64_t& Value);

	UCodeLangExport static bool Field(const StringView FieldName, const int8_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, const int16_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, const int32_t& Value);
	UCodeLangExport static bool Field(const StringView FieldName, const int64_t& Value);

	UCodeLangExport static bool Fieldptr(const StringView FieldName, const uintptr_t& Value);
	UCodeLangExport static bool Fieldptr(const StringView FieldName, const intptr_t& Value);

	UCodeLangExport static bool Field(const StringView FieldName, const bool& Value);

	UCodeLangExport static bool Field(const StringView FieldName, const float& Value);
	UCodeLangExport static bool Field(const StringView FieldName, const double& Value);


	static bool Field(const StringView FieldName, const void* Value, uintptr_t idtype);
	static bool FieldRawView(const StringView FieldName, const void* Value, uintptr_t idtype);
	//
	UCodeLangExport static void BeginReadOnly();

	UCodeLangExport static void EndReadOnly();
};

UCodeAPIEditorEnd