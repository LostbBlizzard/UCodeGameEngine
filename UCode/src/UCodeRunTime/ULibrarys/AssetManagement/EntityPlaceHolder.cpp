#include "EntityPlaceHolder.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetPtr.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/RttrSerializer.hpp"
#include "UCodeRunTime/ULibrarys/Others/StringHelper.hpp"
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
	ApplyChanges();

	#if UCodeGEPublishMode
	this->Destroy(this);	
	#endif
}


using PlaceHolderChangeProps_t = int;
enum class PlaceHolderChangeProps :PlaceHolderChangeProps_t
{
	This,
	Compents,
	RemoveCompent,
	AddCompent,

	Max,
};


std::array<StringView, (PlaceHolderChangeProps_t)PlaceHolderChangeProps::Max> PlaceHolderChangePropsNames =
{
	"this",
	"compents",
	"removecompent",
};

StringView GetPropsName(PlaceHolderChangeProps val)
{
	return PlaceHolderChangePropsNames[(PlaceHolderChangeProps_t)val];
}
constexpr const char* NewValueFieldName = "_";
void EntityPlaceHolder::OnOverrideSerializeEntity(UCode::Scene2dData::Entity_Data& Scene,USerializerType type)
{	
	UpdateChanges(type);
}
void EntityPlaceHolder::UpdateChanges(USerializerType type,EntityPlaceHolderChanges* Out, Entity* entity, NullablePtr<Entity> rawentityop)
{
	if (rawentityop.has_value())
	{
		auto& rawasentity = rawentityop.value();
		auto& rawcompents = rawasentity->NativeCompoents();
		auto& rawentitys = rawasentity->NativeGetEntitys();


		auto& compents = entity->NativeCompoents();
		auto& entitys = entity->NativeGetEntitys();
		for (auto& compoent : compents)
		{
			NullablePtr<Compoent> fromraw;
			{
				for (auto& Item : rawcompents)
				{
					if (Item->Get_CompoentTypeData() == compoent->Get_CompoentTypeData())
					{
						fromraw = Item.get();
						break;
					}
				}
			}

			UpdateChangesCompoentState state;
			state.Out = Out;
			state.compoent = compoent.get();
			state.rawcompoent = fromraw;

			if (type == USerializerType::YAML)
			{
				state.compoentref =
					String(GetPropsName(PlaceHolderChangeProps::This)) + "." +
					String(GetPropsName(PlaceHolderChangeProps::Compents)) + '.' + compoent->Get_CompoentTypeData()->_Type;
			}
			else
			{
				BitConverter tep(BitConverter::Endian::little);

				state.compoentref.resize(sizeof(PlaceHolderChangeProps) + sizeof(PlaceHolderChangeProps));

				tep.MoveBytes((PlaceHolderChangeProps_t)PlaceHolderChangeProps::This, state.compoentref.data(), 0);

				tep.MoveBytes((PlaceHolderChangeProps_t)PlaceHolderChangeProps::Compents, state.compoentref.data(), sizeof(PlaceHolderChangeProps));

				auto& typenmaestr = compoent->Get_CompoentTypeData()->_Type;
				tep.MoveBytes((BitMaker::SizeAsBits)typenmaestr.size(), state.compoentref.data(), sizeof(PlaceHolderChangeProps) * 2);
				state.compoentref += typenmaestr;
			}

			UpdateChanges(type, state);
		}
	
		
		for (auto& Item : rawcompents)
		{
			auto str = Item->Get_CompoentTypeData()->_Type;

			bool hascompent = false;
			for (auto& Item : compents)
			{
				if (Item->Get_CompoentTypeData()->_Type == str)
				{
					hascompent = true;
					break;
				}
			}

			if (!hascompent)
			{
				EntityPlaceHolderChanges::Change change;
				change.field = String(GetPropsName(PlaceHolderChangeProps::This)) + ".";
				change.field += String(GetPropsName(PlaceHolderChangeProps::RemoveCompent));
				change.NewValue = Item->Get_CompoentTypeData()->_Type;
	

				Out->_changes.push_back(std::move(change));
				break;
			}
		}
	}
	else
	{

	}
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

			UpdateChanges(type, &out, NativeEntity(),Nullableptr(rawasentity));	
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

				issame = RttrSerializer::IsSame(Item.get_type(), compentfield, rawfield);
			}
			

			if (!issame)
			{
				tep.Reset();
				EntityPlaceHolderChanges::Change change;
				
				if (type == USerializerType::YAML) 
				{
					change.field = state.compoentref + "." + Item.get_name();

					RttrSerializer::Write(tep,NewValueFieldName,compentfield);
					tep.ToString(change.NewValue, false);
				}
				else if (type == USerializerType::Bytes) 
				{
					BitConverter tepbyte(BitConverter::Endian::little);
					change.field = state.compoentref; 

					auto& str =StringView(Item.get_name().data(), Item.get_name().size());
					
					change.field.resize(change.field.size() + sizeof(BitMaker::SizeAsBits));
					tepbyte.MoveBytes((BitMaker::SizeAsBits)str.size(),change.field.data(),change.field.size());
					change.field += str;

					RttrSerializer::Write(tep,NewValueFieldName,compentfield);
					tep.ToString(change.NewValue, false);
				}

				state.Out->_changes.push_back(std::move(change));
			}
		}
	}
	
}
void EntityPlaceHolder::ApplyChanges()
{
	auto& in = _change;

	auto& compents = this->NativeEntity()->NativeCompoents();
	auto& entitys = this->NativeEntity()->NativeGetEntitys();

	Vector<StringView> parts;
	for (auto& Item : in._changes)
	{
		parts.clear();
		StringHelper::Split(Item.field, ".", parts);

		if (parts.size() >= 1)
		{
			auto& first = parts[0];

			if (first == GetPropsName(PlaceHolderChangeProps::This))
			{
				if (parts.size() >= 2)
				{
					auto& sec = parts[1];
					if (sec ==  GetPropsName(PlaceHolderChangeProps::Compents))
					{
						if (parts.size() >= 3)
						{
							auto& compenttype = parts[2];

							NullablePtr<Compoent> valop;
							for (auto& Item : compents)
							{
								if (Item->Get_CompoentTypeData()->_Type == compenttype)
								{
									valop = Nullableptr(Item.get());
									break;
								}
							}

							if (valop.has_value())
							{
								auto val = valop.value();

								if (parts.size() >= 4)
								{
									auto& field = parts[3];

									auto& runtimetypereflection = val->Get_CompoentTypeData()->_RuntimeTypeReflection.value();

									for (auto& pro : runtimetypereflection.get_properties())
									{
										auto v = pro.get_name();
										if (StringView(v.data(), v.size()) == field)
										{
											auto instance = val->Get_Rttr_Instance();

											UCode::UDeserializer deser(Item.NewValue);
											RttrSerializer::Read(deser, NewValueFieldName,instance,pro);
										}
									}
								}

							}
						}
					}
					else if (sec == GetPropsName(PlaceHolderChangeProps::RemoveCompent))
					{
						auto& toremove = Item.NewValue;

						for (auto& Item : compents)
						{
							if (Item->Get_CompoentTypeData()->_Type == toremove)
							{
								Compoent::Destroy(Item.get());
								break;
							}
						}
					}
				}
			}
		}
	}
}
CoreEnd
