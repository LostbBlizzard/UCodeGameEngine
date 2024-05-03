#include "AssetRendering.hpp"
#include "UCodeRunTime/CoreSystems/Threads.hpp"

#include <stb_image/stb_image.h>
CoreStart
void SpriteData::PushData(USerializer& node) const
{
	node.Write("_TextureUID", _Texture);
	node.Write("_OffsetX", OffsetX);
	node.Write("_OffsetY", OffsetY);
	node.Write("_SizeX", SizeX);
	node.Write("_SizeY", SizeY);
}

bool SpriteData::FromString(SpriteData& out, UDeserializer& text)
{
	text.ReadType("_TextureUID", out._Texture, out._Texture);
	text.ReadType("_OffsetX", out.OffsetY, out.OffsetX);
	text.ReadType("_OffsetY", out.OffsetY, out.OffsetY);
	text.ReadType("_SizeX", out.SizeX, out.SizeX);
	text.ReadType("_SizeY", out.SizeY, out.SizeY);
	return true;
}

bool SpriteData::FromFile(SpriteData& out, const Path& Path)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V);
	if (A)
	{
		return FromString(out, V);
	}
	return A;
}

bool SpriteData::ToFile(const Path& path, const SpriteData& data, USerializerType Type)
{
	USerializer V(Type);
	data.PushData(V);
	return V.ToFile(path);
}
void TextureData::PushData(USerializer& node) const
{
	node.Write("_ReadWrite", ReadWrite);
	node.Write("_PixelPerUnit", PixelPerunit);
	node.Write("_Filter", (Texture::Filter_t)Filter);
	node.Write("_TextureType", _TextureType);
	node.Write("_TextureData", _TextureData);
}

bool TextureData::FromString(TextureData& out, UDeserializer& text)
{
	text.ReadType("_ReadWrite", out.ReadWrite, out.ReadWrite);
	text.ReadType("_PixelPerUnit", out.PixelPerunit, out.PixelPerunit);
	text.ReadType("_Filter", *(Texture::Filter_t*)&out.Filter, *(Texture::Filter_t*)&out.Filter);
	text.ReadType("_TextureType", out._TextureType, out._TextureType);
	text.ReadType("_TextureData", out._TextureData, out._TextureData);
	return true;
}

bool TextureData::FromFile(TextureData& out, const Path& Path)
{
	UDeserializer V;
	bool A = UDeserializer::FromFile(Path, V);
	if (A)
	{
		return FromString(out, V);
	}
	return A;
}

bool TextureData::ToFile(const Path& path, const TextureData& data, USerializerType Type)
{
	USerializer V(Type);
	data.PushData(V);
	return V.ToFile(path);
}
void AssetRendering::UpdatePtr(AssetManager* Manager, TexturePtr& Ptr)
{

}
void AssetRendering::UpdatePtr(AssetManager* Manager, SpritePtr& Ptr)
{
	if (Ptr.Get_State() == SpritePtr::State::ManagedPtr)
	{
		if (!Ptr.Has_Asset())
		{
			//asset was Distory
			Ptr = {};
		}
		else
		{
			auto man = Ptr.Get_Managed();

			int a = 0;
		}
	}
	else if (Ptr.Has_UID())
	{
		UID ID = Ptr.Get_UID();
		auto asset = Manager->FindOrLoad(ID);
		if (asset)
		{
			auto TypedAsset = asset.value().Get_Value()->GetAssetAs<SpriteAsset>();
			if (TypedAsset.has_value()) {
				Ptr = TypedAsset.value()->GetManaged();
			}
		}
	}
}
void AssetRendering::UpdatePtr(AssetManager* Manager, ShaderPtr& Ptr)
{
}
void AssetRendering::DrawQuad2d(RenderRunTime2d* runtime, const DrawQuad2dData& Data)
{

	AssetManager* Manager = AssetManager::Get(runtime->GetGameLib());
	RenderRunTime2d::DrawQuad2dData _Data(Data.pos, Data.size, Data.rotation);



	Sprite* Spr = nullptr;


	if (Data.Spr.Has_Asset())
	{
		Spr = Data.Spr.Get_Asset();
	}
	else
	{
		if (Data.Spr.Has_UID())
		{
			UID UId = Data.Spr.Get_UID();
			auto R = Manager->FindOrLoad(UId);
			if (R.has_value())
			{
				auto op = R.value().Get_Value()->GetAssetAs<SpriteAsset>();

				if (op.has_value()) {
					Spr = &op.value()->_Base;
				}
			}
		}
	}

	_Data.Spr = Spr;

	_Data.color = Data.color;
	_Data.drawLayer = Data.drawLayer;
	_Data.draworder = Data.draworder;
#if UCodeGEDebug
	_Data.madeby = Data.madeby;
#endif // DEBUG
	runtime->DrawQuad2d(_Data);

}
AsynTask_t<Unique_ptr<Texture>> AssetRendering::LoadTextureAsync(Gamelibrary* lib, const Path& path)
{
	Threads* threads = Threads::Get(lib);


	Delegate<Unique_ptr<Texture>> Func = [path = path, lib]()
		{
			GameFiles* f = GameFiles::Get(lib);

			auto teptex = new Texture(f->ReadGameFileAsBytes(path).AsView());

			return Unique_ptr<Texture>(teptex);
		};


	return threads->AddTask_t(TaskType::Rendering,
		std::move(Func), {});
}
AsynTask_t<Unique_ptr<Texture>> AssetRendering::LoadTextureAsync(Gamelibrary* lib, const BytesView bits)
{
	Threads* threads = Threads::Get(lib);

	Unique_Bytes NewBits;
	NewBits.Copyfrom(bits);
	Vector<Byte> Bytes = NewBits.MoveToVector();

	Delegate<Unique_ptr<Texture>> Func = [Bits = std::move(Bytes)]() mutable
		{

			auto teptex = new Texture();
			teptex->SetTexture(BytesView::Make(Bits.data(), Bits.size()));


			return Unique_ptr<Texture>(teptex);
		};

	Delegate<Unique_ptr<Texture>, Unique_ptr<Texture>> Func2 = [](Unique_ptr<Texture>&& Tex) mutable
		{
			Tex->InitTexture();

			return std::move(Tex);
		};


	return threads->AddTask_t(TaskType::DataProcessing, std::move(Func), {})
		.ContinueOnThread<Unique_ptr<Texture>>(TaskType::Rendering, std::move(Func2));
}
CoreEnd