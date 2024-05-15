#include "SceneData.hpp"
#include "UCodeRunTime/ULibrarys/Rendering_Library.hpp"

#include "UCodeRunTime/ULibrarys/UCodeLang/ULangRunTime.hpp"

#include "UCodeRunTime/ULibrarys/Serialization/Bit_Implementation/AsssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/Yaml_Implementation.hpp"
#include "UCodeRunTime/Core/UModule.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/EntityPlaceHolder.hpp"
CoreStart
Scene2dData::Scene2dData()
{
}
void Scene2dData::PushData(USerializer &node)
{
    node.Write("_Name", _Name);
    node.Write("_UID", _UID);
    node.Write("_Entitys", _Entitys);
}

bool Scene2dData::FromString(Scene2dData &out, UDeserializer &text)
{
    out._Entitys.clear();

    text.ReadType("_Name", out._Name, out._Name);
    text.ReadType("_UID", out._UID, out._UID);
    text.ReadType("_Entitys", out._Entitys, out._Entitys);

    return true;
}
void Scene2dData::SaveScene(const RunTimeScene *Scene, Scene2dData &Out, USerializerType type)
{
    Out._Name = Scene->Get_Name();
    Out._UID = Scene->Get_UID();

    const auto &Entitys = Scene->Get_Entitys();

    Out._Entitys.clear();
    Out._Entitys.reserve(Entitys.size());

    for (const auto &Item : Entitys)
    {
        Entity_Data Entity;
        SaveEntityData(Item.get(), Entity, type);
        Out._Entitys.push_back(std::move(Entity));
    }
}
void Scene2dData::SaveEntityData(const Entity* Item, Scene2dData::Entity_Data& Entity, USerializerType type)
{
    Entity._LocalPosition = Item->NativeLocalPosition();
    Entity._LocalRotation = Item->NativeLocalRotation();
    Entity._LocalScale = Item->NativeLocalScale();

    Entity._Name = Item->NativeName();
        
        
    if (auto v = Item->GetCompent<EntityPlaceHolder>().value_unchecked())
    {
        v->OnOverrideSerializeEntity(Entity, type);

        Compoent_Data Data; 
        if (!SaveCompoentData(v, Data, type))
        {
            return;
        }
        Entity._Compoents.push_back(std::move(Data));
    }
    else
    {
        const auto& Compoents = Item->NativeCompoents();
        Entity._Compoents.reserve(Compoents.size());
        for (const auto& Item : Compoents)
        {
            Compoent_Data Data;

            if (!SaveCompoentData(Item.get(), Data, type))
            {
                continue;
            }

            Entity._Compoents.push_back(std::move(Data));
        }

        const auto& Entitys = Item->NativeGetEntitys();
        Entity._Entitys.reserve(Entitys.size());

        for (const auto& Item : Entitys)
        {
            Entity_Data EntityItem;
            SaveEntityData(Item.get(), EntityItem, type);
            Entity._Entitys.push_back(std::move(EntityItem));
        }
    }
}
bool Scene2dData::SaveCompoentData(const Compoent *Item, Compoent_Data &Out, USerializerType type)
{
    const auto &TypeId = Item->Get_CompoentTypeData()->_Type;
    USerializer serializer = USerializer(type);

    if (!SaveCompoentDataAsText(Item, serializer))
    {
        return false;
    }

    Out._CompoentType = TypeId;
    serializer.ToString(Out._Data, true);
    return true;
}
bool Scene2dData::SaveCompoentDataAsText(const Compoent *Item, USerializer &serializer)
{
    auto TypeId = Item->Get_CompoentTypeData()->_Type;
    Item->Serialize(serializer);
    return true;
}
RunTimeScene *Scene2dData::LoadScene(GameRunTime *runtime, const Scene2dData &Data)
{
	UCodeGEDebugStackFrame("SceneData::LoadScene");
    RunTimeScene *r = runtime->Add_NewScene();
    r->Get_Name() = Data._Name;
    r->Get_UID() = Data._UID;

    for (const auto &Item : Data._Entitys)
    {
        Entity *Entity = r->NewEntity();
        LoadEntity(Entity, Item);
    }

    return r;
}

RunTimeScene *Scene2dData::AsyncLoadScene(GameRunTime *runtime, const Scene2dData &Data, bool AsRunAsMultThread)
{
    RunTimeScene *r = runtime->Add_NewScene();
    UCodeGEToDo();
    return r;
}
void Scene2dData::LoadEntity(Entity *entity, const Entity_Data &Item)
{
    entity->NativeLocalPosition() = Item._LocalPosition;
    entity->NativeLocalRotation() = Item._LocalRotation;
    entity->NativeLocalScale() = Item._LocalScale;
    entity->NativeName() = Item._Name;

    for (const auto &C : Item._Compoents)
    {
        Compoent *Compoent = LoadCompoent(entity, C);
    }

    for (const auto &C : Item._Entitys)
    {
        Entity* Ptr = entity->NativeAddEntity();
        LoadEntity(Ptr, C);
    }
}
Compoent *Scene2dData::LoadCompoent(Entity *Entity, const Compoent_Data &Item)
{
    UDeserializer serializer;
    serializer.SetData(BytesView((Byte *)Item._Data.data(), Item._Data.size()));

    Compoent *Compoent;

    auto ComponetData = UModules::GetComponet(Item._CompoentType);
    if (ComponetData && ComponetData->TypeData && ComponetData->TypeData->_NewComponent)
    {
        auto Compoent = ComponetData->TypeData->_NewComponent(Entity);
        Compoent->Deserialize(serializer);
        Entity->MoveCompoent(Compoent);

        return Compoent;
    }
    return nullptr;
}
bool Scene2dData::FromFile(Scene2dData &out, const Path &Path)
{
    UDeserializer V;
    bool A = UDeserializer::FromFile(Path, V);
    if (A)
    {
        return FromString(out, V);
    }
    return A;
}
bool Scene2dData::ToFile(const Path &path, Scene2dData &data, USerializerType Type)
{
    USerializer V(Type);
    data.PushData(V);
    return V.ToFile(path);
}
Entity *Scene2dData::CloneEntity(Entity *entity, RunTimeScene *Scene)
{
    UCode::Scene2dData::Entity_Data Data;
    UCode::Scene2dData::SaveEntityData(entity, Data, USerializerType::Fastest);

    auto R = Scene->NewEntity();
    UCode::Scene2dData::LoadEntity(R, Data);
    return R;
}
CoreEnd