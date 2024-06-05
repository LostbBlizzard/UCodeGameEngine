#include "ULangWindow.hpp"
EditorStart 

ULangWindow::ULangWindow(const NewEditorWindowData& windowdata) 
	:EditorWindow(windowdata)
{
}
ULangWindow::~ULangWindow()
{
    if (HasScript())
    {
        UnLoadScript();
    }
}
void ULangWindow::UpdateWindow()
{
    if (HasScript() && _LangUpdate)
    {
        auto current = UC::UCodeRunTimeState::Get_Current();

        current->LangThisCall(_LangUpdate, _UObj);
    }
}
void ULangWindow::OnSaveWindow(USerializer& Input)
{
    UCodeGEDebugStackFrame("ULangScript::Serialize");

	if (HasScript())
	{
	    Input.Write("_ScriptType", _ClassName);
		void* Ptr = Get_UObj();
		auto ClassType = Get_ClassData();

		bool Is32Mode = sizeof(void*) == 4;

		UC::ULangHelper::Serialize(Input , Ptr, *ClassType, GetULang()->Get_Assembly(),Is32Mode);
	}
}
void ULangWindow::OnLoadWindow(UDeserializer& Input)
{
    UCodeGEDebugStackFrame("ULangScript::Deserialize");
	String ScriptType = NoClassScript;
	Input.ReadType("_ScriptType", ScriptType, ScriptType);
	if (ScriptType != NoClassScript)
	{
		SetClassName(ScriptType);
		if (HasScript())
		{

			void* Ptr = Get_UObj();
			auto ClassType = Get_ClassData();

			bool Is32Mode = sizeof(void*) == 4;

			UC::ULangHelper::Deserialize(Input, Ptr, *ClassType, GetULang()->Get_Assembly(), Is32Mode);
		}
	}

}
void ULangWindow::UnLoadScript()
{
    UCodeGEDebugStackFrame("ScirptableObject::UnLoadScript");
    if (_LangDestructor)
    {
        UC::UCodeRunTimeState::LangThisCall(_LangDestructor, _UObj);
    }
    UC::UCodeRunTimeState::Get_Current()->Lang_Free(_UObj);
    
    _UObj = nullptr;
    _ClassData = nullptr; 
    _LangDestructor = nullptr;
    _LangConstructor = nullptr;
    _LangUpdate = nullptr;
}
void ULangWindow::LoadScript(const UCodeLang::AssemblyNode* scriptnode)
{
    UCodeGEDebugStackFrame("ULangWindow::LoadScript");
    const UCodeLang::Class_Data* ClassData = &scriptnode->Get_ClassData();
    if (_ClassData)
    {
        UnLoadScript();
    }
    _ClassData = ClassData;

    GetCallBinds();
    auto runstate = UC::UCodeRunTimeState::Get_Current();

    UCodeLang::ReflectionTypeInfo type;
    type._Type = UCodeLang::ReflectionTypes::CustomType;
    type._CustomTypeID = _ClassData->TypeID;

    _UObj = runstate->Lang_Malloc(runstate->Get_Assembly().GetSize(type, sizeof(void*) == 4).value());

    _ClassName = scriptnode->FullName;
    if (_LangConstructor)
    {
        runstate->LangThisCall(_LangConstructor, _UObj);
    }
}
EditorWindowData ULangWindow::GetEditorData()
{
	return EditorWindowData("ULangWindow", MakeWin);
}
EditorWindow* ULangWindow::MakeWin(const NewEditorWindowData& project)
{
	return new ULangWindow(project);
}
	

EditorEnd