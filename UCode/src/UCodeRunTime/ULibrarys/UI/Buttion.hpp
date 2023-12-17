#pragma once

#include "UISpriteRenderer.hpp"
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
		_ClickCallBack = Value;
	}
	inline void Remove_OnClickCallBack()
	{
		UCodeGEToDo();
	}

	// UIInfo
	inline auto &Get_UIData() { return _Renderer->UIData; }
	inline auto &Get_shader() { return _Renderer->shader; }
	inline auto &Get_texture() { return _Renderer->texture; }
	inline auto &Get_color() { return _Renderer->color; }
	inline auto &Get_flipX() { return _Renderer->flipX; }
	inline auto &Get_flipY() { return _Renderer->flipY; }
	inline auto &Get_DrawOrder() { return _Renderer->DrawOrder; }

private:
	void UIAPI_OnClick();
	UISpriteRenderer *_Renderer;
	ClickCallBackType _ClickCallBack;
	static UComponentData type_Data;
};

UIEnd