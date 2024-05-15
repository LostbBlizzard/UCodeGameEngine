#include "EntityPlaceHolder.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetRendering.hpp"
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

	Optional<Assetptr> assetop = AssetManager::Get(GetGameRunTime()->Get_Library_Edit())->FindOrLoad(_id);
	
	
	bool itworked = false;
	if (assetop.has_value())
	{
		auto rawop = assetop.value().Get_Value()->GetAssetAs<RawEntityDataAsset>();

		if (rawop.has_value())
		{
			const auto rawasentity = this->Get_Scene()->Get_RunTime()->NewEntityOnRunTimeScene();
			Entity::Destroy(rawasentity);
			UCode::Scene2dData::LoadEntity(rawasentity, rawop.value()->_Base._Data);

			out._changes.clear();
			itworked = true;
			
			auto& rawcompents = rawasentity->NativeCompoents();
			auto& rawentitys = rawasentity->NativeGetEntitys();

			
			auto& compents = this->NativeEntity()->NativeCompoents();
			auto& entitys = this->NativeEntity()->NativeGetEntitys();
			for (size_t i = 0; i < compents.size(); i++)
			{
				auto copent = compents[i].get();
				NullablePtr<Compoent> fromraw;
				{
					for (auto& Item : rawcompents)
					{
						if (Item->Get_CompoentTypeData() == copent->Get_CompoentTypeData())
						{
							fromraw = Item.get();
							break;
						}
					}
				}

				UpdateChangesCompoentState state;
				state.Out = &out;
				state.compoent = copent;
				state.rawcompoent = fromraw;
				state.compoentref = "this.compents." + copent->Get_CompoentTypeData()->_Type;	
				UpdateChanges(type, state);
			}

		}

	}


	if (itworked == false)
	{
		//I dont know what to do if AssetPtr is null
	}
}
void EntityPlaceHolder::UpdateChanges(USerializerType type,UpdateChangesCompoentState state)
{
	USerializer tep(type);

	if (state.compoent->Get_CompoentTypeData()->_RuntimeTypeReflection.has_value())
	{
		auto& runtimetypereflection = state.compoent->Get_CompoentTypeData()->_RuntimeTypeReflection.value();

		for (auto& Item : runtimetypereflection.get_properties())	
		{
			auto compentfield = Item.get_value(state.compoent->Get_Rttr_Instance());


			bool issame = false;
			if (state.rawcompoent.has_value())
			{
				auto rawfield = Item.get_value(state.rawcompoent.value()->Get_Rttr_Instance());

				//C++ Base Types
				if (Item.get_type() == rttr::type::get<int>())
				{
					issame = compentfield.get_value<int>() == rawfield.get_value<int>();
				}
				else if (Item.get_type() == rttr::type::get<bool>())
				{
					issame = compentfield.get_value <bool>() == rawfield.get_value<bool>();
				}

				//C++ Stl Types

				//UCode Types
				else if (Item.get_type() == rttr::type::get<Color>())
				{
					issame = compentfield.get_value<Color>() == rawfield.get_value<Color>();
				}

				//AssetTypes 
				else if (Item.get_type() == rttr::type::get<SpritePtr>())
				{
					issame = compentfield.get_value<SpritePtr>() == rawfield.get_value<SpritePtr>();
				}
				else
				{
					UCodeGEUnreachable();
				}
			}
			

			if (!issame)
			{
				EntityPlaceHolderChanges::Change change;
				change.field = state.compoentref + "." + Item.get_name();
				change.NewValue = "0";
				state.Out->_changes.push_back(std::move(change));
			}
		}
	}
	
}
void EntityPlaceHolder::AppleChanges()
{

}
CoreEnd
