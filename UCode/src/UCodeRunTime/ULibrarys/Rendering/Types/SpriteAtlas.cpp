#include "SpriteAtlas.hpp"

RenderingStart
SpriteAtlas::SpriteAtlas(Texture* Texture, Sprite* Sprites, size_t Sprites_Length) :
	_Texture(Texture)
{
	_Sprites.reserve(Sprites_Length);
	for (size_t i = 0; i < Sprites_Length; i++)
	{
		_Sprites.push_back(Sprites[i]);
	}
}

RenderingEnd

