#pragma once
#include <UCodeRunTime/ULibrarys/Rendering/RenderRunTime2d.hpp>
#include "AssetManager.hpp"
#include "AssetPtr.hpp"
#include <UCodeRunTime/BasicTypes.hpp>
#include "UCodeRunTime/CoreSystems/Threads.hpp"
CoreStart



struct TextureAsset : public Asset
{
public:
	TextureAsset(Texture&& base) :_Base(std::move(base))
	{

	}
	Texture _Base;

	ManagedPtr<TextureAsset> GetManaged()
	{
		auto v =this->Get_Managed();

		return *(ManagedPtr<TextureAsset>*)&v;
	}
};
using TextureAssetPtr = ManagedPtr<TextureAsset>;
using TexturePtr= AssetPtr<TextureAsset,Texture>;

struct SpriteData
{
	inline static const char* FileExt = "USprite";
	inline static const char* FileExtDot = ".USprite";

	UID _Texture;
	i32 OffsetX = 0;
	i32 OffsetY = 0;
	i32 SizeX = 0;
	i32 SizeY = 0;

	void PushData(USerializer& node) const;
	static bool FromString(SpriteData& out, UDeserializer& text);

	static bool FromFile(SpriteData& out, const Path& Path);
	static bool ToFile(const Path& path,const SpriteData& data, USerializerType Type);
};
struct TextureData
{
	inline static const char* FileExt = "UTexture";
	inline static const char* FileExtDot = ".UTexture";

	bool ReadWrite = false;
	u32 PixelPerunit = 0;
	Texture::Filiter Filter = Texture::Filiter::Point;

	String _TextureType;
	Vector<Byte> _TextureData;

	void PushData(USerializer& node) const;
	static bool FromString(TextureData& out, UDeserializer& text);

	static bool FromFile(TextureData& out, const Path& Path);
	static bool ToFile(const Path& path,const TextureData& data, USerializerType Type);
};

class SpriteAsset : public Asset
{
public:
	
	SpriteAsset(const Sprite & spr,const TextureAssetPtr& ptr)
		:_Base(std::move(spr)),_Texture(std::move(ptr))
	{

	}
	Sprite _Base;
	TextureAssetPtr _Texture;

	ManagedPtr<SpriteAsset> GetManaged()
	{
		auto v =this->Get_Managed();
		return *(ManagedPtr<SpriteAsset>*)&v;
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
		auto V =this->Get_Managed();
		return *(ManagedPtr<SharderAsset>*)&V;
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
#if UCodeGEDebug
		Renderer2d* madeby = nullptr;
#endif // DEBUG
		DrawQuad2dData(Vec2 p, Vec2 s, Vec2 rot)
			:pos(p), size(s), rotation(rot), Spr(nullptr), color(Color())
			, drawLayer(RenderRunTime2d::DrawLayer_Min)
			, draworder(RenderRunTime2d::DrawOrderType_Min)
		{

		};
		UCodeGEForceinlne void SetTexture(SpritePtr v) { Spr = v; }
		UCodeGEForceinlne void SetColor(Color v) { color = v; }
		UCodeGEForceinlne void SetDrawLayer(RenderRunTime2d::DrawLayer_t v) { drawLayer = v; }
		UCodeGEForceinlne void SetDraworder(RenderRunTime2d::DrawOrder_t v) { draworder = v; }


	};
	static void UpdatePtr(AssetManager* Manager,TexturePtr& Ptr);
	static void UpdatePtr(AssetManager* Manager,SpritePtr& Ptr);
	static void UpdatePtr(AssetManager* Manager,ShaderPtr& Ptr);
	static void DrawQuad2d(RenderRunTime2d* runtime,const DrawQuad2dData& Data);
	static AsynTask_t<Unique_ptr<Texture>> LoadTextureAsync(Gamelibrary* lib, const Path& path);
	static AsynTask_t<Unique_ptr<Texture>> LoadTextureAsync(Gamelibrary* lib, const BytesView bits);
};
CoreEnd