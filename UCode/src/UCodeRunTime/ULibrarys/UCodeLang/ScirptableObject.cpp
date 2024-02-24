#include "ScirptableObject.hpp"
LangStart 


void ScirptableObjectData::PushData(USerializer& node) const
{
    node.Write("_UID", _UID);
    node.Write("_ObjectType", _ObjectType);
    if (node.Get_Type() == USerializerType::YAML)
    {
        auto& yaml = node.Get_TextMaker();
        yaml << YAML::Key << "_Data" << YAML::Value;
        
        yaml << YAML::Load(_Data);
    }
    else 
    {
        node.Write("_Data", _Data);
    }
}

bool ScirptableObjectData::FromString(ScirptableObjectData& out, UDeserializer& text)
{
    text.ReadType("_UID", out._UID, out._UID);
    text.ReadType("_ObjectType", out._ObjectType, out._ObjectType);
    text.ReadType("_Data", out._Data, out._Data);
	return true;
}

bool ScirptableObjectData::FromFile(ScirptableObjectData& out, const Path& Path)
{
    UDeserializer V;
    bool A = UDeserializer::FromFile(Path, V);
    if (A)
    {
        return FromString(out, V);
    }
    return A;
}

bool ScirptableObjectData::ToFile(const Path& path, ScirptableObjectData& data, USerializerType Type)
{
    USerializer V(Type);
    data.PushData(V);
    return V.ToFile(path);
}

void ScirptableObject::LoadScript(const UCodeLang::AssemblyNode* scriptnode)
{
    const UCodeLang::Class_Data* ClassData = &scriptnode->Get_ClassData();

    if (_ClassData)
    {
        UnLoadScript();
    }
    _ClassData = ClassData;

    GetCallBinds();
    auto runstate = UCodeRunTimeState::Get_Current();

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
void ScirptableObject::LoadScript(const ScirptableObjectData& out)
{ 
    if (_ClassData)
    {
        UnLoadScript();
    } 
    
    GetCallBinds();
	UCodeGEToDo();
}
void ScirptableObject::UnLoadScript()
{
    if (_LangDestructor)
    {
        UCodeRunTimeState::LangThisCall(_LangDestructor, _UObj);
    }
    UCodeRunTimeState::Get_Current()->Lang_Free(_UObj);
    
    _UObj = nullptr;
    _ClassData = nullptr; 
    _LangDestructor = nullptr;
    _LangConstructor = nullptr;
}
void ScirptableObject::SaveTo(ScirptableObjectData& out, USerializerType type) const
{
    out._ObjectType = _ClassName;

    USerializer serlalizer(type);

    ULangHelper::Serialize(serlalizer, _UObj, *_ClassData,UCodeRunTimeState::Get_Current()->Get_Assembly(), sizeof(void*) == 4);

    serlalizer.ToString(out._Data,false);
}
LangEnd

