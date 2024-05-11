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
	#if UCodeGEPublishMode
	this->Destroy(this);	
	#endif
}
void EntityPlaceHolder::Serialize(USerializer& Serializer) const 
{

}
void EntityPlaceHolder::Deserialize(UDeserializer& Serializer)
{

}


CoreEnd
