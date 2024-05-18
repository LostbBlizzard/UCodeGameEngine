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
EntityPlaceHolder::~EntityPlaceHolder()
{
	#if UCodeGEDebugMode
	if (evenid.has_value())
	{
		RemoveAssetUpdateEditorEvent(evenid.value());
	}
	#endif
}

void EntityPlaceHolder::Start()
{
	
}
void EntityPlaceHolder::Serialize(USerializer& Serializer) const 
{
	Serializer.Write("_UID", _id);
	Serializer.Write("_Changes",_change);
}


void EntityPlaceHolder::OnUpdatedID()
{
	#if UCodeGEDebugMode
	if (evenid.has_value())
	{
		RemoveAssetUpdateEditorEvent(evenid.value());
	}
	evenid = AddAssetUpdatedEditorEvent(
		[this]  
		{
			this->OnAssetPreUpdate();
		},
		[this]
		{
			this->OnAssetUpdated(); 
		}, _id);
	#endif
}

void EntityPlaceHolder::OnAssetPreUpdate()
{
	#if UCodeGEDebugMode
	if (this->Get_IsDestroyed()) { return; }

	_oldentitydata = Scene2dData::Entity_Data();

	Scene2dData::SaveEntityData(NativeEntity(), this->_oldentitydata, USerializerType::YAML);
	#endif
}
void EntityPlaceHolder::OnAssetUpdated()
{
	#if UCodeGEDebugMode
	if (this->Get_IsDestroyed()) { return; }

	for (auto& Item : NativeEntity()->NativeGetEntitys())
	{
		Entity::Destroy(Item.get());
	}
	for (auto& Item : NativeEntity()->NativeCompoents())
	{
		Compoent::Destroy(Item.get());
	}

	Scene2dData::LoadEntity(NativeEntity(), this->_oldentitydata);
	#endif
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
	OnUpdatedID();
	#if UCodeGEPublishMode
	this->Destroy(this);	
	#endif
}


using PlaceHolderChangeProps_t = int;
enum class PlaceHolderChangeProps :PlaceHolderChangeProps_t
{
	This,
	Compents,
	Entitys,
	RemoveCompent,
	RemoveEntity,
	AddCompent,
	AddEntity,


	Max,
};


std::array<StringView, (PlaceHolderChangeProps_t)PlaceHolderChangeProps::Max> PlaceHolderChangePropsNames =
{
	"this",
	"compents",
	"entitys",
	"removecompent",
	"removeentity",
	"addcompent",
	"addentity",
};

StringView GetPropsName(PlaceHolderChangeProps val)
{
	return PlaceHolderChangePropsNames[(PlaceHolderChangeProps_t)val];
}
constexpr const char* NewValueFieldName = "_";
void EntityPlaceHolder::OnOverrideSerializeEntity(UCode::Scene2dData::Entity_Data& Scene,USerializerType type)
{
	Entity* raw;
	UpdateChanges(type,&raw);

	{
		auto& rawcompents = raw->NativeCompoents();
		auto& compents = NativeEntity()->NativeCompoents();
		for (auto& Item : compents)
		{
			if (Item->Get_IsDestroyed()) { continue; }
			auto& typen = Item->Get_CompoentTypeData()->_Type;

			bool isadded = true;
			for (auto& Item : rawcompents)
			{
				if (typen == Item->Get_CompoentTypeData()->_Type)
				{
					isadded = false;
					break;
				}
			}

			if (isadded)
			{
				if (Item.get() != this) 
				{
					UCode::Scene2dData::Compoent_Data data;

					UCode::Scene2dData::SaveCompoentData(Item.get(), data, type);

					Scene._Compoents.push_back(std::move(data));
				}

			}
		}

	}
{
		auto& rawcompents = raw->NativeGetEntitys();
		auto& compents = NativeEntity()->NativeGetEntitys();
		for (auto& Item : compents)
		{
			if (Item->Get_IsDestroyed()) { continue; }
			auto& typen = Item->NativeName();

			bool isadded = true;
			for (auto& Item : rawcompents)
			{
				if (typen == Item->NativeName())
				{
					isadded = false;
					break;
				}
			}

			if (isadded)
			{
				UCode::Scene2dData::Entity_Data data;

				UCode::Scene2dData::SaveEntityData(Item.get(), data, type);

				Scene._Entitys.push_back(std::move(data));
			}
		}

	}
}
void EntityPlaceHolder::UpdateChanges(USerializerType type,EntityPlaceHolderChanges* Out, Entity* entity, NullablePtr<Entity> rawentityop,String changestart)
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
			if (compoent->Get_IsDestroyed()) { continue; }
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
					changestart + "." +
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

			if (!fromraw.has_value())
			{

			}
			else 
			{
				UpdateChanges(type, state);
			}
		}
		for (auto& Item : entitys)
		{
			NullablePtr<Entity> RawEntity;
			{
				for (auto& raw : rawentitys)
				{
					if (raw->NativeName() == Item->NativeName())
					{
						RawEntity = Nullableptr(raw.get());
						break;
					}
				}
			}

			if (RawEntity.has_value())
			{
				auto raw = RawEntity.value();

				auto entityid = changestart + "."+  String(GetPropsName(PlaceHolderChangeProps::Entitys)) + String(".") + Item->NativeName() + String(".");
				if (Item->GetActive() != raw->GetActive())
				{
					EntityPlaceHolderChanges::Change change;
					change.field = entityid + String("Active");

					USerializer tep(type);
					tep.Write(NewValueFieldName, Item->GetActive());
					tep.ToString(change.NewValue, false);

					Out->_changes.push_back(std::move(change));
				}
				if (Item->NativeLocalPosition() != raw->NativeLocalPosition())
				{
					EntityPlaceHolderChanges::Change change;
					change.field = entityid + String("LocalPosition");

					USerializer tep(type);
					tep.Write(NewValueFieldName, Item->LocalPosition());
					tep.ToString(change.NewValue, false);

					Out->_changes.push_back(std::move(change));
				}
				if (Item->NativeLocalScale() != raw->NativeLocalScale())
				{
					EntityPlaceHolderChanges::Change change;
					change.field = entityid + String("LocalScale");

					USerializer tep(type);
					tep.Write(NewValueFieldName, Item->NativeLocalScale());
					tep.ToString(change.NewValue, false);

					Out->_changes.push_back(std::move(change));
				}
				if (Item->NativeLocalRotation() != raw->NativeLocalRotation())
				{
					EntityPlaceHolderChanges::Change change;
					change.field = entityid + String("LocalRotation");

					USerializer tep(type);
					tep.Write(NewValueFieldName, Item->NativeLocalRotation());
					tep.ToString(change.NewValue, false);

					Out->_changes.push_back(std::move(change));
				}

				UpdateChanges(type, Out, Item.get(), RawEntity, entityid);
			}
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
				change.field = changestart + ".";
				change.field += String(GetPropsName(PlaceHolderChangeProps::RemoveCompent));
				change.NewValue = Item->Get_CompoentTypeData()->_Type;
	

				Out->_changes.push_back(std::move(change));
				break;
			}
		}
		for (auto& Item : rawentitys)
		{
			auto str = Item->NativeName();

			bool hascompent = false;
			for (auto& Item : entitys)
			{
				if (Item->NativeName() == str)
				{
					hascompent = true;
					break;
				}
			}

			if (!hascompent)
			{
				EntityPlaceHolderChanges::Change change;
				change.field = changestart + ".";
				change.field += String(GetPropsName(PlaceHolderChangeProps::RemoveEntity));
				change.NewValue = Item->NativeName();
	

				Out->_changes.push_back(std::move(change));
				break;
			}
		}
	}
	else
	{

	}
}
void EntityPlaceHolder::UpdateChanges(USerializerType type,Entity** rawentity)
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

			*rawentity = rawasentity;
			out._changes.clear();
			itworked = true;

			UpdateChanges(type, &out, NativeEntity(),Nullableptr(rawasentity),String(GetPropsName(PlaceHolderChangeProps::This)));
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



				if (!issame)
				{
					tep.Reset();
					EntityPlaceHolderChanges::Change change;

					if (type == USerializerType::YAML)
					{
						change.field = state.compoentref + "." + Item.get_name();

						RttrSerializer::Write(tep, NewValueFieldName, compentfield);
						tep.ToString(change.NewValue, false);
					}
					else if (type == USerializerType::Bytes)
					{
						BitConverter tepbyte(BitConverter::Endian::little);
						change.field = state.compoentref;

						auto& str = StringView(Item.get_name().data(), Item.get_name().size());

						change.field.resize(change.field.size() + sizeof(BitMaker::SizeAsBits));
						tepbyte.MoveBytes((BitMaker::SizeAsBits)str.size(), change.field.data(), change.field.size());
						change.field += str;

						RttrSerializer::Write(tep, NewValueFieldName, compentfield);
						tep.ToString(change.NewValue, false);
					}

					state.Out->_changes.push_back(std::move(change));
				}
			}
		}
	}
	
}
void EntityPlaceHolder::ApplyChanges()
{
	auto& in = _change;



	Vector<StringView> parts;
	for (auto& Item : in._changes)
	{
		parts.clear();
		StringHelper::Split(Item.field, ".", parts);

		Entity* e = this->NativeEntity();

		bool donewithloop = false;
		size_t Index = 0;
		while (donewithloop == false)
		{
			auto& compents = e->NativeCompoents();
			auto& entitys = e->NativeGetEntitys();


			if (parts.size() >= Index + 1)
			{
				auto& first = parts[Index + 0];

				if (first == GetPropsName(PlaceHolderChangeProps::This))
				{
					e = this->NativeEntity();
					Index++;
				}
				else if (first == GetPropsName(PlaceHolderChangeProps::Compents))
				{
					if (parts.size() >= Index + 1)
					{
						auto& compenttype = parts[Index + 1];

						NullablePtr<Compoent> valop;
						for (auto& Item : compents)
						{
							if (Item->Get_CompoentTypeData()->_Type == compenttype && !Item->Get_IsDestroyed())
							{
								valop = Nullableptr(Item.get());
								break;
							}
						}

						if (valop.has_value())
						{
							auto val = valop.value();

							if (parts.size() >=  Index + 2)
							{
								auto& field = parts[Index + 2];

								auto& runtimetypereflection = val->Get_CompoentTypeData()->_RuntimeTypeReflection.value();

								for (auto& pro : runtimetypereflection.get_properties())
								{
									auto v = pro.get_name();
									if (StringView(v.data(), v.size()) == field)
									{
										auto instance = val->Get_Rttr_Instance();

										UCode::UDeserializer deser(Item.NewValue);
										RttrSerializer::Read(deser, NewValueFieldName, instance, pro);
										break;
									}
								}
								donewithloop = true;
							}

						}
					}
				}
				else if (first == GetPropsName(PlaceHolderChangeProps::RemoveCompent))
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
					donewithloop = true;
				}
				else if (first == GetPropsName(PlaceHolderChangeProps::RemoveEntity))
				{
					auto& toremove = Item.NewValue;

					for (auto& Item : entitys)
					{
						if (Item->NativeName()== toremove)
						{
							Entity::Destroy(Item.get());
							break;
						}
					}
					donewithloop = true;
				}
				else if (first == GetPropsName(PlaceHolderChangeProps::Entitys))
				{
					if (parts.size() >= Index + 1)
					{
						auto& toget = parts[Index + 1];
						for (auto& Item : e->NativeGetEntitys())
						{
							if (Item->NativeName() == toget && !Item->Get_IsDestroyed())
							{
								e = Item.get();
								break;
							}
						}
						Index+=2;
					}
				}
				else if (first == "LocalPosition")
				{
					Vec3& out =e->NativeLocalPosition();
					UCode::UDeserializer deser(Item.NewValue);
					deser.ReadType(NewValueFieldName, out,out);
					donewithloop = true;
				}
				else if (first == "LocalRotation")
				{
					Vec3& out =e->NativeLocalRotation();
					UCode::UDeserializer deser(Item.NewValue);
					deser.ReadType(NewValueFieldName, out,out);
					donewithloop = true;
				}
				else if (first == "LocalScale")
				{
					Vec3& out =e->NativeLocalScale();
					UCode::UDeserializer deser(Item.NewValue);
					deser.ReadType(NewValueFieldName, out,out);
					donewithloop = true;
				}
				else if (first == "Active")
				{
					bool r = e->GetActive();
					UCode::UDeserializer deser(Item.NewValue);
					deser.ReadType(NewValueFieldName, r,r);
					e->SetActive(r);
					donewithloop = true;
				}
			}
		}
	}
}
CoreEnd
