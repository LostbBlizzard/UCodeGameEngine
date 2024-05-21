#pragma once
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/CoreRunTime.hpp>
#include "RawEntityData.hpp"
#include "SceneData.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/SerlizeableType.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/USerializer.hpp"
#include "UCodeRunTime/ULibrarys/EditorEvents/AssetUpdateEvent.hpp"
CoreStart
struct EntityPlaceHolderChanges
{
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

		ActiveEntity,
		LocalPositionEntity,
		LocalScaleEntity,
		LocalRotationEntity,

		Max,
	};


	inline const static std::array<StringView, (PlaceHolderChangeProps_t)PlaceHolderChangeProps::Max> PlaceHolderChangePropsNames
		=
	{
		"this",
		"compents",
		"entitys",
		"removecompent",
		"removeentity",
		"addcompent",
		"addentity",
		"active",
		"localposition",
		"localscale",
		"localRotation",
	};

	static StringView GetPropsName(PlaceHolderChangeProps val)
	{
		return PlaceHolderChangePropsNames[(PlaceHolderChangeProps_t)val];
	}
	static Optional<PlaceHolderChangeProps> GetProp(StringView val)
	{
		for (size_t i = 0; i < PlaceHolderChangePropsNames.size(); i++)
		{
			auto& Item = PlaceHolderChangePropsNames[i];
			if (Item == val)
			{
				return (PlaceHolderChangeProps)i;
			}
		}
		return {};
	}
	struct Change
	{
		String field;
		String NewValue;

		struct GetIndexChash
		{
			Optional<Vector<StringView>> parts;
			Optional<BitReader> reader;
		};

		using MemberRet = Variant<PlaceHolderChangeProps, StringView>;

		void AddField(USerializerType type, PlaceHolderChangeProps props);
		void AddField(USerializerType type, StringView value);
		void AddField(USerializerType type, size_t value);
		MemberRet GetIndex(USerializerType type, size_t I) const
		{
			GetIndexChash chash = GetIndexChash();
			return GetIndex(type, I);
		}
		size_t IndexCount(USerializerType type) const
		{
			return IndexCount(type);
		}

		MemberRet GetIndex(USerializerType type, size_t I, GetIndexChash& chash) const;
		size_t IndexCount(USerializerType type, GetIndexChash& chash) const;
	};

	Vector<Change> _changes;
	USerializerType _serializertype = USerializerType::Default;
	bool HasChanges()
	{
		return _changes.size() != 0;
	}
};
CoreEnd

MakeSerlizeType(UCode::EntityPlaceHolderChanges::Change,
	Field("_field", _this->field);
Field("_newvalue", _this->NewValue);
)
MakeSerlizeType(UCode::EntityPlaceHolderChanges,
	Field("_Changes", _this->_changes);
)

CoreStart
class EntityPlaceHolder final : public Compoent
{
public:
	EntityPlaceHolder(Entity* entity);
	~EntityPlaceHolder();
	void Start() override;

	void Serialize(USerializer& Serializer) const override;
	void Deserialize(UDeserializer& Serializer) override;
	void OnOverrideSerializeEntity(UCode::Scene2dData::Entity_Data& Scene, USerializerType type);
	UID _id;
	EntityPlaceHolderChanges _change;
	static UComponentData type_Data;

	void OnUpdatedID();

	void UpdateChanges(USerializerType type);
	void RemoveChanges();


	struct GetEntityData
	{
		Entity* entity;
		EntityPlaceHolderChanges::PlaceHolderChangeProps props;
	};
	Optional<GetEntityData> GetEntity(USerializerType type, const EntityPlaceHolderChanges::Change& change);
	struct GetCompoentData
	{
		Compoent* compoent;
		EntityPlaceHolderChanges::PlaceHolderChangeProps props;
	};
	Optional<GetCompoentData> GetCompoent(USerializerType type, const EntityPlaceHolderChanges::Change& change);
private:

	void OnAssetPreUpdate();
	void OnAssetUpdated();
#if UCodeGEDebugMode
	Optional<EditorEventID> evenid;
	UCode::Scene2dData::Entity_Data _oldentitydata;
#endif
	static UComponentsID Get_TypeID();

	void UpdateChanges(USerializerType type, Entity** rawentity);

	struct UpdateChangesCompoentState
	{
		EntityPlaceHolderChanges* Out;
		Compoent* compoent = nullptr;
		NullablePtr<Compoent> rawcompoent;

		EntityPlaceHolderChanges::Change compoentref;
	};

	void UpdateChanges(USerializerType type, UpdateChangesCompoentState state);
	void ApplyChanges();
	void UpdateChanges(USerializerType type, EntityPlaceHolderChanges* Out, Entity* entity, NullablePtr<Entity> rawentityop, String changestart = "");
};
CoreEnd