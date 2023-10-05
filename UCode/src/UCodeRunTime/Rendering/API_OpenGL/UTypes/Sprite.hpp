#pragma once

#include "Texture.hpp"

RenderingStart

class Sprite
{
public:
	Sprite(Texture* texture, i32 x, i32 y, i32 width, i32 height);





	//gets
	inline auto Get_texture() const { return _texture; }
	inline auto Get_Xoffset() const { return _X; }
	inline auto Get_Yoffset()const { return  _Y; }
	inline auto Get_Width()const { return  _Width; }
	inline auto Get_Height() const { return _Height; }

	
	inline auto Set_texture(Texture* texture) {	_texture = texture;}
	inline auto Set_Xoffset(i32 X) { _X = X; }
	inline auto Set_Yoffset(i32 Y){  _Y =Y; }
	inline auto Set_Width(i32 W){ _Width = W; }
	inline auto Set_Height(i32 H) { _Height = H; }

	static constexpr i32 GetTexureSize = -1;
private:
	Texture* _texture;
	i32 _X;
	i32 _Y;
	i32 _Width;
	i32 _Height;
};
RenderingEnd
