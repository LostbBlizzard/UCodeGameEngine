#pragma once
#include "ULangRunTime.hpp"
CoreStart
struct ScirptableObjectData
{
	inline static const char* FileExt = "UScirptableObject";
	inline static const char* FileExtDot = ".UScirptableObject";

	void PushData(USerializer& node) const;
	static bool FromString(ScirptableObjectData& out, UDeserializer& text);

	static bool FromFile(ScirptableObjectData& out, const Path& Path);
	static bool ToFile(const Path& path, ScirptableObjectData& data, USerializerType Type);

	UID _UID;
	String _ObjectType;
	String _Data;	
	USerializerType _DataSerializeType = USerializerType::Default;
};

class ScirptableObject
{
private:
	ManagedPtr<ScirptableObject> _Managed;

	const UCodeLang::Class_Data* _ClassData = nullptr;
	String _ClassName;
	UCodeLang::PtrType _UObj = nullptr;
public:
	ScirptableObject(){}
	~ScirptableObject(){}
	ManagedPtr<ScirptableObject> Get_ManagedPtr()
	{
		return _Managed;
	}

	UCodeGEForceinlne ULangRunTime* GetULang() const
	{
		return UCodeRunTimeState::Get_Current();
	}

	UCodeGEForceinlne void SetClassName(const String& Name)
	{
		_ClassName = Name;
	}
	UCodeGEForceinlne bool HasScript()const
	{
		return _UObj;
	}
	UCodeGEForceinlne bool HasClass()const
	{
		return _ClassData;
	}
	UCodeGEForceinlne const UCodeLang::Class_Data* Get_ClassData() const
	{
		return  _ClassData;
	}
	UCodeGEForceinlne void* Get_UObj() const
	{
		return _UObj;
	}
	void LoadScript(const UCodeLang::AssemblyNode* scriptnode);

	void SaveTo(ScirptableObjectData& out, USerializerType type) const;
	void LoadScript(const ScirptableObjectData& obj);
	void UnLoadScript();
	
	auto& GetClassName() const
	{
		return _ClassName;
	}

	void** Get_UObjPtr()
	{
		return &_UObj;
	}
 	const UCodeLang::Class_Data** Get_ClassDataPtr()
	{
		return &_ClassData;
	}
private:
	const UCodeLang::ClassMethod* _LangConstructor = nullptr;
	const UCodeLang::ClassMethod* _LangDestructor = nullptr;
	void GetCallBinds()
	{
		auto& Class = *_ClassData;
		_LangConstructor = Class.Get_ClassConstructor();
		_LangDestructor = Class.Get_ClassDestructor();
	}
};

UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $Asset trait:
	  uintptr _Handle = 0;
    ")");

UCodeLangExportSymbol("UCodeGameEngine") UCodeLangEmbed(
	R"(
    $MenuItem<(Size)> tag:
	  char[/Size] MenuName;
      |new[this&,char[/Size] name]:
       MenuName = name;
    ")");


class AssetAPI
{
	static ScirptableObject& Cast(uintptr_t _Handle)
	{
		return *(ScirptableObject*)_Handle;
	}
	static const ScirptableObject& iCast(uintptr_t _Handle)
	{
		return *(const ScirptableObject*)_Handle;
	}

};



CoreEnd
