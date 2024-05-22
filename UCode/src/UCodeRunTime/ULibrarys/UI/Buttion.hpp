#pragma once

#include "UIImage.hpp"
#include "UINamespace.hpp"
UIStart

	class Buttion final : public Compoent
{
public:
	typedef Delegate<void> ClickCallBackType;

	Buttion(Entity *e);
	~Buttion();

	inline void Set_OnClickCallBack(ClickCallBackType Value)
	{

	}
	inline void Remove_OnClickCallBack()
	{
		UCodeGEToDo();
	}

	static UComponentsID Get_TypeID();
	static UComponentData type_Data;
private:
	void UIAPI_OnClick();
};

UIEnd