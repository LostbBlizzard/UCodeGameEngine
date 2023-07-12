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

	UCODE_ENGINE_FORCE_INLINE ULangRunTime* GetULang() const
	{
		return UCodeRunTimeState::Get_Current();
	}

	UCODE_ENGINE_FORCE_INLINE void SetClassName(const String& Name)
	{
		_ClassName = Name;
	}
	UCODE_ENGINE_FORCE_INLINE bool HasScript()const
	{
		return _UObj;
	}
	UCODE_ENGINE_FORCE_INLINE bool HasClass()const
	{
		return _ClassData;
	}
	UCODE_ENGINE_FORCE_INLINE const UCodeLang::Class_Data* Get_ClassData() const
	{
		return  _ClassData;
	}
	UCODE_ENGINE_FORCE_INLINE void* Get_UObj() const
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

