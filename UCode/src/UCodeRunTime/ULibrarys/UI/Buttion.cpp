#include "Buttion.hpp"
UIStart

UComponentData Buttion::type_Data = { "Buttion",[](Entity* E) {return (Compoent*)new Buttion(E); } };
Buttion::Buttion(Entity* e) : Compoent(e,&type_Data)
{

}

Buttion::~Buttion()
{
}


UComponentsID Buttion::Get_TypeID()
{
	return Buttion::type_Data._Type;
}

void Buttion::UIAPI_OnClick()
{
}
UIEnd