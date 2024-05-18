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
	struct Change
	{
		String field;
		String NewValue;
	};

	Vector<Change> _changes;
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
	void OnOverrideSerializeEntity(UCode::Scene2dData::Entity_Data& Scene,USerializerType type);
	UID _id;
	EntityPlaceHolderChanges _change;
	static UComponentData type_Data;

	void OnUpdatedID();

	void UpdateChanges(USerializerType type);
	void RemoveChanges();
private:

	void OnAssetPreUpdate();
	void OnAssetUpdated();
	#if UCodeGEDebugMode
	Optional<EditorEventID> evenid;
	UCode::Scene2dData::Entity_Data _oldentitydata;
	#endif
	static UComponentsID Get_TypeID();

	void UpdateChanges(USerializerType type,Entity** rawentity);

	struct UpdateChangesCompoentState
	{
		EntityPlaceHolderChanges* Out;
		Compoent* compoent = nullptr;
		NullablePtr<Compoent> rawcompoent;

		String compoentref;
	};

	void UpdateChanges(USerializerType type,UpdateChangesCompoentState state);
	void ApplyChanges();
	void UpdateChanges(USerializerType type, EntityPlaceHolderChanges* Out, Entity* entity, NullablePtr<Entity> rawentityop,String changestart = "");
};
CoreEnd