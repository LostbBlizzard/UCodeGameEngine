#pragma once
#include "APINameSpace.hpp"
#include "UCodeLang/RunTime/ReflectionStl.hpp"
#include "../../Typedefs.hpp";
UCodeAPIEditorStart

UCodeLangExportSymbol("UCodeGameEngineEditor") UCodeLangEmbed(
	R"(
    $Window trait:
	  uintptr _Handle = 0;
    ")");

UCodeLangExportSymbol("UCodeGameEngineEditor") UCodeLangEmbed(
	R"(
    $OverWriteTypeDraw trait;
    ")");

UCodeLangExportSymbol("UCodeGameEngineEditor") UCodeLangEmbed(
	R"(
    $WindowDrawAPI:
	  |Field[imut StringSpan FieldName,uint8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	  |Field[imut StringSpan FieldName,uint16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	  |Field[imut StringSpan FieldName,uint32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	  |Field[imut StringSpan FieldName,uint64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

	  
	  |Field[imut StringSpan FieldName,int8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	  |Field[imut StringSpan FieldName,int16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	  |Field[imut StringSpan FieldName,int32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);
	  |Field[imut StringSpan FieldName,int64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Field(FieldName,Value);

	  |Field[imut StringSpan FieldName,uintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);
	  |Field[imut StringSpan FieldName,sintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::Fieldptr(FieldName,Value);

	  |Field<T>[imut StringSpan FieldName,T& Value] => false;
      //Readonly

	  |iField[imut StringSpan FieldName,imut uint8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iField(FieldName,Value);
	  |iField[imutStringSpan FieldName,imut uint16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iField(FieldName,Value);
	  |iField[imut StringSpan FieldName,imut uint32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iField(FieldName,Value);
	  |iField[imut StringSpan FieldName,imut uint64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iField(FieldName,Value);

	  
	  |iField[imut StringSpan FieldName,imut int8& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iField(FieldName,Value);
	  |iField[imut StringSpan FieldName,imut int16& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iField(FieldName,Value);
	  |iField[imut StringSpan FieldName,imut int32& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iField(FieldName,Value);
	  |iField[imut StringSpan FieldName,imut int64& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iField(FieldName,Value);

	  |iField[imut StringSpan FieldName,imut uintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iFieldptr(FieldName,Value);
	  |iField[imut StringSpan FieldName,imut sintptr& Value] => UCodeGameEngineEditorInternal::WindowDrawAPI::iFieldptr(FieldName,Value);

	  |iField<T>[imut StringSpan FieldName,T& Value] => false;
    ")");

UCodeLangExportSymbol("UCodeGameEngineEditorInternal") struct WindowDrawAPI
{
	bool Field(const StringView FieldName, uint8_t& Value);
	bool Field(const StringView FieldName, uint16_t& Value);
	bool Field(const StringView FieldName, uint32_t& Value);
	bool Field(const StringView FieldName, uint64_t& Value);

	bool Field(const StringView FieldName, int8_t& Value);
	bool Field(const StringView FieldName, int16_t& Value);
	bool Field(const StringView FieldName, int32_t& Value);
	bool Field(const StringView FieldName, int64_t& Value);

	bool Fieldptr(const StringView FieldName,uintptr_t& Value);
	bool Fieldptr(const StringView FieldName,intptr_t& Value);

	bool Field(const StringView FieldName, bool& Value);

	bool Field(const StringView FieldName, float& Value);
	bool Field(const StringView FieldName, double& Value);
	
	bool Field(const StringView FieldName,void* Value, UCodeLang::ReflectionCustomTypeID id);
	bool FieldRawView(const StringView FieldName, void* Value, UCodeLang::ReflectionCustomTypeID id);
	//
	bool iField(const StringView FieldName,const uint8_t& Value);
	bool iField(const StringView FieldName,const uint16_t& Value);
	bool iField(const StringView FieldName,const uint32_t& Value);
	bool iField(const StringView FieldName,const uint64_t& Value);

	bool iField(const StringView FieldName,const int8_t& Value);
	bool iField(const StringView FieldName,const int16_t& Value);
	bool iField(const StringView FieldName,const int32_t& Value);
	bool iField(const StringView FieldName,const int64_t& Value);

	bool iFieldptr(const StringView FieldName,const uintptr_t& Value);
	bool iFieldptr(const StringView FieldName,const intptr_t& Value);

	bool iField(const StringView FieldName,const bool& Value);

	bool iField(const StringView FieldName,const float& Value);
	bool iField(const StringView FieldName,const double& Value);


	bool iField(const StringView FieldName, const void* Value, UCodeLang::ReflectionCustomTypeID id);
	bool iFieldRawView(const StringView FieldName,const void* Value,UCodeLang::ReflectionCustomTypeID id);
	//
	void BeginReadOnly();

	void EndReadOnly();
};

UCodeAPIEditorEnd