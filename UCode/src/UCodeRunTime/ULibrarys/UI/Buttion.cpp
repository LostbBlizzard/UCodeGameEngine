#include "Buttion.hpp"
UIStart

UComponentData Buttion::type_Data = { "Buttion",[](Entity* E) {return (Compoent*)new Buttion(E); } };
Buttion::Buttion(Entity* e) : Compoent(e,&type_Data), _Renderer(nullptr)
{
	_Renderer = AddCompoent<UISpriteRenderer>();
}

Buttion::~Buttion()
{
	 Destroy(_Renderer); 
}


void Buttion::UIAPI_OnClick()
{
	_ClickCallBack();
}
UIEnd