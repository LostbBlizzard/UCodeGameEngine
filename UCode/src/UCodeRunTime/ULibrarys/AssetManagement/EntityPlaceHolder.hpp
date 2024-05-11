#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/CoreRunTime.hpp>
#include "RawEntityData.hpp"

CoreStart

class EntityPlaceHolder final : public Compoent
{
public:
	struct Changes
	{

	};
	EntityPlaceHolder(Entity* entity);
	void Start() override;

	void Serialize(USerializer& Serializer) const override;
	void Deserialize(UDeserializer& Serializer) override;
	UID _id;
private:
	static UComponentData type_Data;
	static UComponentsID Get_TypeID();
};
CoreEnd