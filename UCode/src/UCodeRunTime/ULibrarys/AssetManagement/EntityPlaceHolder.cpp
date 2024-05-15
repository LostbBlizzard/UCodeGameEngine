#include "EntityPlaceHolder.hpp"

CoreStart

UComponentsID EntityPlaceHolder::Get_TypeID()
{
	return "EntityPlaceHolder";
}
UComponentData EntityPlaceHolder::type_Data = { Get_TypeID(),[](Entity* E) {return (Compoent*)new EntityPlaceHolder(E); }};


EntityPlaceHolder::EntityPlaceHolder(Entity* entity) : Compoent(entity, &type_Data)
{

}

void EntityPlaceHolder::Start()
{
	
}
void EntityPlaceHolder::Serialize(USerializer& Serializer) const 
{
	Serializer.Write("_UID", _id);
	Serializer.Write("_Changes",_change);
}
void EntityPlaceHolder::Deserialize(UDeserializer& Serializer)
{
	Serializer.ReadType("_UID", _id);
	Serializer.ReadType("_Changes", _change);

	Optional<Assetptr> assetop = AssetManager::Get(GetGameRunTime()->Get_Library_Edit())->FindOrLoad(_id);

	if (assetop.has_value())
	{
		auto rawop = assetop.value().Get_Value()->GetAssetAs<RawEntityDataAsset>();
		if (rawop.has_value())
		{
			auto& raw = rawop.value();

			auto e = this->NativeEntity();
			auto oldp = e->NativeLocalPosition();
			auto olds = e->NativeLocalScale();
			auto oldr = e->NativeLocalRotation();

			Scene2dData::LoadEntity(e, raw->_Base._Data);

			e->NativeLocalPosition() = oldp;
			e->NativeLocalScale() = olds;
			e->NativeLocalRotation() = oldr;
		}
	}
	AppleChanges();

	#if UCodeGEPublishMode
	this->Destroy(this);	
	#endif
}

void EntityPlaceHolder::OnOverrideSerializeEntity(UCode::Scene2dData::Entity_Data& Scene,USerializerType type)
{
	UpdateChanges(type);
}

void EntityPlaceHolder::UpdateChanges(USerializerType type)
{
	auto& out = _change;

}
void EntityPlaceHolder::AppleChanges()
{

}
CoreEnd
