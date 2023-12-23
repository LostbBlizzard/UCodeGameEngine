#pragma once
#include <EditorWindows/EditorWindow.hpp>
#include <UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp>
EditorStart



class ULangWindow :public EditorWindow
{
public:
	UCODE_EDITOR_FORCEINLINE UCode::ULangRunTime* GetULang() const
	{
		return nullptr;
	}

	UCODE_EDITOR_FORCEINLINE void SetClassName(const String& Name)
	{
		_ClassName = Name;
	}
	UCODE_EDITOR_FORCEINLINE bool HasScript()const
	{
		return _UObj;
	}
	UCODE_EDITOR_FORCEINLINE bool HasClass()const
	{
		return _ClassData;
	}
	UCODE_EDITOR_FORCEINLINE const UCodeLang::Class_Data* Get_ClassData() const
	{
		return  _ClassData;
	}
	UCODE_EDITOR_FORCEINLINE void* Get_UObj() const
	{
		return _UObj;
	}
	void LoadScript(const UCodeLang::Class_Data* ClassData);
private:
	const UCodeLang::Class_Data* _ClassData = nullptr;
	String _ClassName;
	UCodeLang::PtrType _UObj = nullptr;

	const UCodeLang::ClassMethod* _LangConstructor = nullptr;
	const UCodeLang::ClassMethod* _LangDestructor = nullptr;
	void GetCallBinds()
	{
		auto& Class = *_ClassData;
		_LangConstructor = Class.Get_ClassInit();
		_LangDestructor = Class.Get_ClassDestructor();
	}
};
EditorEnd