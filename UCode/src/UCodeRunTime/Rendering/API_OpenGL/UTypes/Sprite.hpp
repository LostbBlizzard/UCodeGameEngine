#pragma once

#include "Texture.hpp"

RenderingStart

class Sprite
{
public:
	Sprite(Texture* texture, SInt32 x, SInt32 y, SInt32 width, SInt32 height);





	//gets
	inline auto Get_texture() const { return _texture; }
	inline auto Get_Xoffset() const { return _X; }
	inline auto Get_Yoffset()const { return  _Y; }
	inline auto Get_Width()const { return  _Width; }
	inline auto Get_Height() const { return _Height; }

	
	inline auto Set_texture(Texture* texture) {	_texture = texture;}
	inline auto Set_Xoffset(SInt32 X) { _X = X; }
	inline auto Set_Yoffset(SInt32 Y){  _Y =Y; }
	inline auto Set_Width(SInt32 W){ _Width = W; }
	inline auto Set_Height(SInt32 H) { _Height = H; }

	static constexpr SInt32 GetTexureSize = -1;
private:
	Texture* _texture;
	SInt32 _X;
	SInt32 _Y;
	SInt32 _Width;
	SInt32 _Height;
};
RenderingEnd
