#pragma once
#include "../GpuTypes/Sprite.hpp"
#include <UCodeRunTime/RunTimeBasicTypes/Vector.hpp>
RenderingStart


class SpriteAtlas
{
public:
	SpriteAtlas(Texture* Texture, Sprite* Sprites,size_t Sprites_Length);


	inline Texture* Get_Texture() const { return _Texture; }

	inline Sprite* Get_Sprite(size_t i) { return &_Sprites[i]; }
	inline Sprite* Get_SpriteP() { return &_Sprites[0]; }
	inline size_t Get_SpritesLength() { return _Sprites.size(); }
private:
	Texture* _Texture;
	Vector<Sprite> _Sprites;
};
RenderingEnd


