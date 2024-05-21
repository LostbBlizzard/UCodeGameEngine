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
UComponentData EntityPlaceHolder::type_Data = { Get_TypeID(),[](Entity* E) {return (Compoent*)new EntityPlaceHolder(E); } };


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
	Serializer.Write("_Changes", _change);
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
Optional<EntityPlaceHolder::GetEntityData> EntityPlaceHolder::GetEntity(USerializerType type, const EntityPlaceHolderChanges::Change& change)
{
	auto e = NativeEntity();
	
	EntityPlaceHolderChanges::Change::GetIndexChash chash;
	auto count = change.IndexCount(type,chash);
	size_t i = 0;
	EntityPlaceHolderChanges::PlaceHolderChangeProps rprops = EntityPlaceHolderChanges::PlaceHolderChangeProps::Max;
	while (i < count)
	{
		auto val = change.GetIndex(type,i, chash);

		if (auto d = val.IfType<EntityPlaceHolderChanges::PlaceHolderChangeProps>())
		{
			auto props = *d;
			if (props == EntityPlaceHolderChanges::PlaceHolderChangeProps::This)
			{
				e = NativeEntity();
			}
			else if (props == EntityPlaceHolderChanges::PlaceHolderChangeProps::Entitys)
			{
				if (i + 1 < count)
				{
					auto g = change.GetIndex(type,i +1, chash);

					auto entityname = g.GetType<StringView>();

					bool found = false;
					for (auto& Item : e->NativeGetEntitys())
					{
						if (Item->NativeName() == entityname)
						{
							e = Item.get();
							found = true;
							break;
						}
					}
					if (found == false)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			else if (props == EntityPlaceHolderChanges::PlaceHolderChangeProps::AddEntity)
			{
				if (i + 1 < count)
				{
					auto g = change.GetIndex(type, i + 1, chash);

					auto entityname = g.GetType<StringView>();

					bool found = false;
					for (auto& Item : e->NativeGetEntitys())
					{
						if (Item->NativeName() == entityname)
						{
							e = Item.get();
							rprops = props;
							found = true;
							break;
						}
					}
					if (found == false)
					{
						break;
					}
				}
				else
				{
					break;
				}
			}
			else 
			{
				break;
			}
		}
		else
		{
			break;
		}

		i++;
	}
	if (rprops == EntityPlaceHolderChanges::PlaceHolderChangeProps::Max){
		return {};
	}
	GetEntityData r;
	r.entity = e;
	r.props = rprops;
	return r;
}
NullablePtr<Compoent> EntityPlaceHolder::GetCompoent(USerializerType type, const EntityPlaceHolderChanges::Change& change)
{
	return {};
}

void EntityPlaceHolder::RemoveChanges()
{
	for (auto& Item : NativeEntity()->NativeGetEntitys())
	{
		Entity::Destroy(Item.get());
	}
	for (auto& Item : NativeEntity()->NativeCompoents())
	{
		if (Item.get() != this)
		{
			Compoent::Destroy(Item.get());
		}
	}
	Optional<Assetptr> assetop = AssetManager::Get(GetGameRunTime()->Get_Library_Edit())->FindOrLoad(_id);

	if (assetop.has_value())
	{
		auto rawop = assetop.value().Get_Value()->GetAssetAs<RawEntityDataAsset>();
		if (rawop.has_value())
		{
			auto raw = rawop.value();

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

	_change._serializertype = Serializer.Get_Mode();
	if (assetop.has_value())
	{
		auto rawop = assetop.value().Get_Value()->GetAssetAs<RawEntityDataAsset>();
		if (rawop.has_value())
		{
			auto raw = rawop.value();

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



constexpr const char* NewValueFieldName = "_";
void EntityPlaceHolder::OnOverrideSerializeEntity(UCode::Scene2dData::Entity_Data& Scene, USerializerType type)
{
	Entity* raw;
	UpdateChanges(type, &raw);

}
void EntityPlaceHolder::UpdateChanges(USerializerType type, EntityPlaceHolderChanges* Out, Entity* entity, NullablePtr<Entity> rawentityop, String changestart)
{
	bool useindex = type == USerializerType::Bytes;

	if (rawentityop.has_value())
	{
		auto rawasentity = rawentityop.value();
		auto& rawcompents = rawasentity->NativeCompoents();
		auto& rawentitys = rawasentity->NativeGetEntitys();


		auto& compents = entity->NativeCompoents();
		auto& entitys = entity->NativeGetEntitys();
		for (auto& compoent : compents)
		{
			if (compoent->Get_IsDestroyed()) { continue; }
			if (compoent.get() == this) { continue; }
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

			state.compoentref.field = changestart;
			state.compoentref.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::Compents);
			state.compoentref.AddField(type, compoent->Get_CompoentTypeData()->_Type);

			if (!fromraw.has_value())
			{
				EntityPlaceHolderChanges::Change change;
				change.field = changestart;
				change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::AddCompent);
				change.AddField(type, compoent->Get_CompoentTypeData()->_Type);

				USerializer serializer(type);
				Scene2dData::SaveCompoentDataAsText(compoent.get(), serializer);
				serializer.ToString(change.NewValue);
				Out->_changes.push_back(std::move(change));
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


				EntityPlaceHolderChanges::Change change;
				change.field = changestart;
				change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::Entitys);
				change.AddField(type, Item->NativeName());

				auto entityid = change.field;
				if (Item->GetActive() != raw->GetActive())
				{
					change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::ActiveEntity);

					USerializer tep(type);
					tep.Write(NewValueFieldName, Item->GetActive());
					tep.ToString(change.NewValue, false);

					Out->_changes.push_back(std::move(change));
				}
				if (Item->NativeLocalPosition() != raw->NativeLocalPosition())
				{
					change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::LocalPositionEntity);

					USerializer tep(type);
					tep.Write(NewValueFieldName, Item->LocalPosition());
					tep.ToString(change.NewValue, false);

					Out->_changes.push_back(std::move(change));
				}
				if (Item->NativeLocalScale() != raw->NativeLocalScale())
				{
					change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::LocalScaleEntity);

					USerializer tep(type);
					tep.Write(NewValueFieldName, Item->NativeLocalScale());
					tep.ToString(change.NewValue, false);

					Out->_changes.push_back(std::move(change));
				}
				if (Item->NativeLocalRotation() != raw->NativeLocalRotation())
				{
					change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::LocalRotationEntity);

					USerializer tep(type);
					tep.Write(NewValueFieldName, Item->NativeLocalRotation());
					tep.ToString(change.NewValue, false);

					Out->_changes.push_back(std::move(change));
				}

				UpdateChanges(type, Out, Item.get(), RawEntity, entityid);
			}
			else
			{
				EntityPlaceHolderChanges::Change change;
				change.field = changestart;
				change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::AddEntity);
				change.AddField(type, Item->NativeName());

				USerializer serializer(type);

				Scene2dData::Entity_Data data;
				Scene2dData::SaveEntityData(Item.get(), data, type);
				serializer.Write(NewValueFieldName, data);
				serializer.ToString(change.NewValue);

				Out->_changes.push_back(std::move(change));
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
				change.field = changestart;
				change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::RemoveCompent);
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
				change.field = changestart;
				change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::RemoveEntity);
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
void EntityPlaceHolder::UpdateChanges(USerializerType type)
{
	Entity* v;
	UpdateChanges(type, &v);
}
void EntityPlaceHolder::UpdateChanges(USerializerType type, Entity** rawentity)
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

			EntityPlaceHolderChanges::Change change;
			change.AddField(type, EntityPlaceHolderChanges::PlaceHolderChangeProps::This);
			UpdateChanges(type, &out, NativeEntity(), Nullableptr(rawasentity), change.field);
		}

	}


	if (itworked == false)
	{
		//I dont know what to do if AssetPtr is null
	}
}


void EntityPlaceHolder::UpdateChanges(USerializerType type, UpdateChangesCompoentState state)
{
	USerializer tep(type);

	if (state.compoent->Get_CompoentTypeData()->_RuntimeTypeReflection.has_value())
	{
		auto& runtimetypereflection = state.compoent->Get_CompoentTypeData()->_RuntimeTypeReflection.value();

		size_t propindex = 0;
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
					change.field = state.compoentref.field;

					bool useindex = type == USerializerType::Bytes;

					if (useindex)
					{
						change.AddField(type, propindex);
					}
					else
					{
						auto name = StringView(Item.get_name().data(), Item.get_name().size());
						change.AddField(type, name);

						RttrSerializer::Write(tep, NewValueFieldName, compentfield);
						tep.ToString(change.NewValue, false);
					}


					state.Out->_changes.push_back(std::move(change));
				}
			}
			propindex++;
		}
	}

}

size_t EntityPlaceHolderChanges::Change::IndexCount(USerializerType type, GetIndexChash& chash) const
{
	if (type != USerializerType::Bytes)
	{
		if (!chash.parts.has_value())
		{
			chash.parts = StringHelper::Split(field, ".");
		}
		return chash.parts.value().size();
	}
	else
	{
		if (!chash.reader.has_value())
		{
			chash.reader = BitReader((Byte*)field.data(), field.size());
		}
		auto& reader = chash.reader.value();
		reader.Resetoffset();

		EntityPlaceHolderChanges::PlaceHolderChangeProps tep;
		EntityPlaceHolderChanges::PlaceHolderChangeProps_t& tep_t = *(EntityPlaceHolderChanges::PlaceHolderChangeProps_t*)&tep;

		bool endloop = false;
		size_t count = 0;
		while (endloop == false)
		{
			reader.ReadType(tep_t, tep_t);

			switch (tep)
			{
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::This:
				break;
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::Compents:
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::Entitys:
			{
				BitMaker::SizeAsBits val;
				reader.ReadType(val, val);
				reader.Addoffset(val);

				count++;
			}
			break;
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::RemoveCompent:
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::RemoveEntity:
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::AddEntity:
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::AddCompent:
			{
				BitMaker::SizeAsBits val;
				reader.ReadType(val, val);
				reader.Addoffset(val);


				count++;
				endloop = true;
			}
			break;

			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::ActiveEntity:
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::LocalPositionEntity:
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::LocalScaleEntity:
			case UCode::EntityPlaceHolderChanges::PlaceHolderChangeProps::LocalRotationEntity:
				endloop = true;
				break;

			default:
				UCodeGEUnreachable();
				break;
			}

			count++;
		}
		reader.Resetoffset();

		return count;
	}
}
EntityPlaceHolderChanges::Change::MemberRet EntityPlaceHolderChanges::Change::GetIndex(USerializerType type, size_t I, EntityPlaceHolderChanges::Change::GetIndexChash& chash) const
{
	bool isusingstringparts = type != USerializerType::Bytes;
	if (isusingstringparts)
	{
		if (!chash.parts.has_value())
		{
			chash.parts = StringHelper::Split(field, ".");
		}
	}
	else
	{
		if (!chash.reader.has_value())
		{
			chash.reader = BitReader((Byte*)field.data(), field.size());
		}
		chash.reader.value().Resetoffset();
	}

	StringView str;
	Optional<EntityPlaceHolderChanges::PlaceHolderChangeProps> Props;
	bool nextisname = false;
	for (size_t i = 0; i < I + 1; i++)
	{
		bool setnextisstring = false;

		if (!Props.has_value())
		{
			if (nextisname)
			{
				if (isusingstringparts)
				{
					auto& Item = chash.parts.value()[i];
					str = Item;
				}
				else
				{
					auto& red = chash.reader.value();
					BitMaker::SizeAsBits size;
					red.ReadType(size, size);

					auto ptr = red.Get_OffsetDataPointer();
					red.Addoffset(size);

					str = StringView((char*)ptr, size);
				}
				nextisname = false;
			}
			else
			{
				if (isusingstringparts)
				{
					auto& Item = chash.parts.value()[i];
					Props = EntityPlaceHolderChanges::GetProp(Item);
				}
				else
				{
					auto& red = chash.reader.value();
					EntityPlaceHolderChanges::PlaceHolderChangeProps val;
					EntityPlaceHolderChanges::PlaceHolderChangeProps_t& ptr = *(EntityPlaceHolderChanges::PlaceHolderChangeProps_t*)&val;

					red.ReadType(ptr, ptr);
					Props = val;
				}
			}
		}
		else
		{
			auto& p = Props.value();

			if (p == PlaceHolderChangeProps::This)
			{
				if (isusingstringparts)
				{
					auto& Item = chash.parts.value()[i];
					Props = EntityPlaceHolderChanges::GetProp(Item);
				}
				else
				{
					auto& red = chash.reader.value();
					EntityPlaceHolderChanges::PlaceHolderChangeProps val;
					EntityPlaceHolderChanges::PlaceHolderChangeProps_t& ptr = *(EntityPlaceHolderChanges::PlaceHolderChangeProps_t*)&val;

					red.ReadType(ptr, ptr);
					Props = val;
				}
			}
			else if (p == PlaceHolderChangeProps::Compents || p == PlaceHolderChangeProps::Entitys ||
				p == PlaceHolderChangeProps::AddCompent || p == PlaceHolderChangeProps::AddEntity
				|| p == PlaceHolderChangeProps::RemoveEntity || p == PlaceHolderChangeProps::RemoveCompent)
			{
				if (isusingstringparts)
				{
					auto& Item = chash.parts.value()[i];
					str = Item;
				}
				else
				{
					auto& red = chash.reader.value();
					BitMaker::SizeAsBits size;
					red.ReadType(size, size);

					auto ptr = red.Get_OffsetDataPointer();
					red.Addoffset(size);

					str = StringView((char*)ptr, size);
				}
				Props = {};
				setnextisstring = true;
			}
		}

		nextisname = false;
		if (setnextisstring)
		{
			nextisname = true;
		}
	}


	if (Props.has_value())
	{
		return Props.value();
	}
	else
	{
		return str;
	}
}
void EntityPlaceHolderChanges::Change::AddField(USerializerType type, size_t value) 
{
	if (type == USerializerType::YAML)
	{
		if (field.size())
		{
			field += '.';
		}

		field += std::to_string(value);
	}
	else
	{
		BitConverter tep(BitConverter::Endian::little);
		auto oldcount = field.size();
		field.resize(field.size() + sizeof(BitMaker::SizeAsBits));

		tep.MoveBytes((BitMaker::SizeAsBits)value, field.data(), oldcount);
	}
}
void EntityPlaceHolderChanges::Change::AddField(USerializerType type, StringView value)
{
	if (type == USerializerType::YAML)
	{
		if (field.size())
		{
			field += '.';
		}

		field += value;
	}
	else
	{

		BitConverter tep(BitConverter::Endian::little);
		auto oldcount = field.size();
		field.resize(field.size() + sizeof(BitMaker::SizeAsBits) + value.size());

		tep.MoveBytes((BitMaker::SizeAsBits)value.size(), field.data(), oldcount);
		memcpy((void*)((uintptr_t)field.data() + oldcount + sizeof(BitMaker::SizeAsBits)), value.data(), value.size());
	}
}
void EntityPlaceHolderChanges::Change::AddField(USerializerType type, PlaceHolderChangeProps props)
{
	if (type == USerializerType::YAML)
	{
		if (field.size())
		{
			field += '.';
		}

		field += GetPropsName(props);
	}
	else
	{
		BitConverter tep(BitConverter::Endian::little);
		auto oldcount = field.size();
		field.resize(field.size() + sizeof(PlaceHolderChangeProps));

		tep.MoveBytes((PlaceHolderChangeProps_t)props, field.data(), oldcount);
	}
}

void EntityPlaceHolder::ApplyChanges()
{
	auto& in = _change;

	bool UseIndex = in._serializertype == USerializerType::Bytes;
	for (auto& Item : in._changes)
	{
		Entity* e = NativeEntity();

		EntityPlaceHolderChanges::Change::GetIndexChash chash;
		size_t count = Item.IndexCount(in._serializertype, chash);
		size_t i = 0;

		while (i < count)
		{
			auto& compents = e->NativeCompoents();
			auto& entitys = e->NativeGetEntitys();

			auto item = Item.GetIndex(in._serializertype, i, chash);
			using Proptype = EntityPlaceHolderChanges::PlaceHolderChangeProps;

			if (auto val = item.IfType<Proptype>())
			{
				auto& prop = *val;

				if (prop == Proptype::This)
				{
					e = this->NativeEntity();
				}
				else if (prop == Proptype::Compents)
				{
					if (count >= i + 1)
					{
						auto compenttype = Item.GetIndex(in._serializertype, i + 1, chash).GetType<StringView>();

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

							if (count >= i + 2)
							{
								auto field = Item.GetIndex(in._serializertype, i + 2, chash).GetType<StringView>();

								auto& runtimetypereflection = val->Get_CompoentTypeData()->_RuntimeTypeReflection.value();

								for (auto& pro : runtimetypereflection.get_properties())
								{
									auto v = pro.get_name();
									if (StringView(v.data(), v.size()) == field)
									{
										auto instance = val->Get_Rttr_Instance();

										UCode::UDeserializer deser;
										deser.SetData(BytesView((Byte*)Item.NewValue.data(), Item.NewValue.size()), in._serializertype);
										RttrSerializer::Read(deser, NewValueFieldName, instance, pro);
										i += 2;
										break;
									}
								}
							}

						}
					}
				}
				else if (prop == Proptype::RemoveCompent)
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
				else if (prop == Proptype::RemoveEntity)
				{
					auto& toremove = Item.NewValue;

					for (auto& Item : entitys)
					{
						if (Item->NativeName() == toremove)
						{
							Entity::Destroy(Item.get());
							break;
						}
					}
				}
				else if (prop == Proptype::AddCompent)
				{
					if (count >= i + 1)
					{
						UCode::Scene2dData::Compoent_Data val;
						val._CompoentType = Item.GetIndex(in._serializertype, i + 1, chash).GetType<StringView>();
						val._Data = Item.NewValue;

						UCode::Scene2dData::LoadCompoent(e, val);
					}
				}
				else if (prop == Proptype::AddEntity)
				{
					if (count >= i + 1)
					{
						UCode::Scene2dData::Entity_Data val;

						UCode::UDeserializer deser;
						deser.SetData(BytesView((Byte*)Item.NewValue.data(), Item.NewValue.size()), in._serializertype);
						deser.ReadType(NewValueFieldName, val);

						auto newe = e->NativeAddEntity();
						newe->NativeName() = Item.GetIndex(in._serializertype, i + 1, chash).GetType<StringView>();

						UCode::Scene2dData::LoadEntity(newe, val);
					}
				}
				else if (prop == Proptype::Entitys)
				{
					if (count >= i + 1)
					{
						auto toget = Item.GetIndex(in._serializertype, i + 1, chash).GetType<StringView>();
						for (auto& Item : e->NativeGetEntitys())
						{
							if (Item->NativeName() == toget && !Item->Get_IsDestroyed())
							{
								e = Item.get();
								break;
							}
						}
						i++;
					}
				}
				else if (prop == Proptype::LocalPositionEntity)
				{
					Vec3& out = e->NativeLocalPosition();
					UCode::UDeserializer deser;
					deser.SetData(BytesView((Byte*)Item.NewValue.data(), Item.NewValue.size()), in._serializertype);
					deser.ReadType(NewValueFieldName, out, out);
				}
				else if (prop == Proptype::LocalRotationEntity)
				{
					Vec3& out = e->NativeLocalRotation();
					UCode::UDeserializer deser(Item.NewValue);
					deser.SetData(BytesView((Byte*)Item.NewValue.data(), Item.NewValue.size()), in._serializertype);
					deser.ReadType(NewValueFieldName, out, out);
				}
				else if (prop == Proptype::LocalScaleEntity)
				{
					Vec3& out = e->NativeLocalScale();
					UCode::UDeserializer deser;
					deser.SetData(BytesView((Byte*)Item.NewValue.data(), Item.NewValue.size()), in._serializertype);
					deser.ReadType(NewValueFieldName, out, out);
				}
				else if (prop == Proptype::ActiveEntity)
				{
					bool r = e->GetActive();
					UCode::UDeserializer deser;
					deser.SetData(BytesView((Byte*)Item.NewValue.data(), Item.NewValue.size()), in._serializertype);
					deser.ReadType(NewValueFieldName, r, r);
					e->SetActive(r);
				}

			}

			i++;
		}
	}

}
CoreEnd


