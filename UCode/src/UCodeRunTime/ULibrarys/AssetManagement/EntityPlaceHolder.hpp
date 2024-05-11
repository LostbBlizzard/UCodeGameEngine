#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/CoreRunTime.hpp>
#include "RawEntityData.hpp"
#include "SceneData.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/SerlizeableType.hpp"
#include "UCodeRunTime/ULibrarys/Serialization/USerializer.hpp"
CoreStart
struct EntityPlaceHolderChanges
{
	using ChangeType_t = int;
	enum class ChangeType : ChangeType_t 
	{
		Set,
		Add,
		Sub,
	};
	struct Change
	{
		String field;

	};

	Vector<Change> _changes;
};
CoreEnd

MakeSerlizeType(UCode::EntityPlaceHolderChanges::Change,
	Field("_field", _this->field);
)
MakeSerlizeType(UCode::EntityPlaceHolderChanges,
	Field("_Changes", _this->_changes);
)

CoreStart
class EntityPlaceHolder final : public Compoent
{
public:
	EntityPlaceHolder(Entity* entity);
	void Start() override;

	void Serialize(USerializer& Serializer) const override;
	void Deserialize(UDeserializer& Serializer) override;
	void OnOverrideSerializeEntity(UCode::Scene2dData::Entity_Data& Scene,USerializerType type);
	UID _id;
	EntityPlaceHolderChanges _change;
	static UComponentData type_Data;
private:
	static UComponentsID Get_TypeID();
	void UpdateChanges(USerializerType type);
	void AppleChanges();
};
CoreEnd