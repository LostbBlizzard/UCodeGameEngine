#pragma once
#include <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include "AssetManager.hpp"
#include "AssetPtr.hpp"
#include <UCodeRunTime/BasicTypes.hpp>
#include "UCodeRunTime/CoreBooks/BookOfThreads.hpp"
CoreStart



struct TextureAsset : public Asset
{
public:
	Texture _Base;

	ManagedPtr<TextureAsset> GetManaged()
	{
		return *(ManagedPtr<TextureAsset>*)&this->Get_Managed();
	}
};
using TextureAssetPtr = ManagedPtr<TextureAsset>;
using TexturePtr= AssetPtr<TextureAsset,Texture>;


class SpriteAsset : public Asset
{
public:
	Sprite _Base;
	TextureAsset _Texture;

	ManagedPtr<SpriteAsset> GetManaged()
	{
		return *(ManagedPtr<SpriteAsset>*)&this->Get_Managed();
	}
};
using SpritePtr =AssetPtr<SpriteAsset,Sprite>;
using SpriteAssetPtr = ManagedPtr<SpriteAsset>;

struct SharderAsset : public Asset
{
public:
	Shader _Base;

	ManagedPtr<SharderAsset > GetManaged()
	{
		return *(ManagedPtr<SharderAsset>*) &this->Get_Managed();
	}
};
using ShaderAssetPtr = ManagedPtr<SharderAsset>;
using ShaderPtr = AssetPtr<SharderAsset,Shader>;


class AssetRendering
{
public:
	struct DrawQuad2dData
	{
	public:
		Vec2 pos;
		Vec2 size;
		Vec2 rotation;
		SpritePtr Spr;
		Color color;
		RenderRunTime2d::DrawLayer_t drawLayer;
		RenderRunTime2d::DrawOrder_t draworder;
#if UCodeGameEngineDEBUG
		Renderer2d* madeby = nullptr;
#endif // DEBUG
		DrawQuad2dData(Vec2 p, Vec2 s, Vec2 rot)
			:pos(p), size(s), rotation(rot), Spr(nullptr), color(Color())
			, drawLayer(RenderRunTime2d::DrawLayer_Min)
			, draworder(RenderRunTime2d::DrawOrderType_Min)
		{

		};
		UCodeGameEngineForceinlne void SetTexture(SpritePtr v) { Spr = v; }
		UCodeGameEngineForceinlne void SetColor(Color v) { color = v; }
		UCodeGameEngineForceinlne void SetDrawLayer(RenderRunTime2d::DrawLayer_t v) { drawLayer = v; }
		UCodeGameEngineForceinlne void SetDraworder(RenderRunTime2d::DrawOrder_t v) { draworder = v; }


	};
	static void UpdatePtr(AssetManager* Manager,TexturePtr& Ptr);
	static void UpdatePtr(AssetManager* Manager,SpritePtr& Ptr);
	static void UpdatePtr(AssetManager* Manager,ShaderPtr& Ptr);
	static void DrawQuad2d(RenderRunTime2d* runtime,const DrawQuad2dData& Data);
	static AsynTask_t<Unique_ptr<Texture>> LoadTextureAsync(Gamelibrary* lib, const Path& path);
	static AsynTask_t<Unique_ptr<Texture>> LoadTextureAsync(Gamelibrary* lib, const BytesView bits);
};
CoreEnd