#pragma once
#include "ULangRunTime.hpp"
LangStart
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

	UCodeGameEngineForceinlne ULangRunTime* GetULang() const
	{
		return UCodeRunTimeState::Get_Current();
	}

	UCodeGameEngineForceinlne void SetClassName(const String& Name)
	{
		_ClassName = Name;
	}
	UCodeGameEngineForceinlne bool HasScript()const
	{
		return _UObj;
	}
	UCodeGameEngineForceinlne bool HasClass()const
	{
		return _ClassData;
	}
	UCodeGameEngineForceinlne const UCodeLang::Class_Data* Get_ClassData() const
	{
		return  _ClassData;
	}
	UCodeGameEngineForceinlne void* Get_UObj() const
	{
		return _UObj;
	}
	void LoadScript(const UCodeLang::Class_Data* ClassData);
private:
	const UCodeLang::ClassMethod* _LangConstructor = nullptr;
	const UCodeLang::ClassMethod* _LangDestructor = nullptr;
	void GetCallBinds()
	{
		auto& Class = *_ClassData;
		_LangConstructor = Class.Get_ClassInit();
		_LangDestructor = Class.Get_ClassDestructor();
	}
};
LangEnd

