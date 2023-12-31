#include "AssetRendering.hpp"
#include "UCodeRunTime/CoreSystems/Threads.hpp"

#include <stb_image/stb_image.h>
CoreStart
void AssetRendering::UpdatePtr(AssetManager* Manager,TexturePtr& Ptr)
{
	
}
void AssetRendering::UpdatePtr(AssetManager* Manager,SpritePtr& Ptr)
{
	if (Ptr.Has_UID())
	{
		UID ID = Ptr.Get_UID();
		auto asset = Manager->FindAsset(ID);
		if (asset)
		{
			auto TypedAsset = asset.value().Get_Value()->GetAssetAs<SpriteAsset>();
			Ptr = TypedAsset->GetManaged();
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
				Spr = &R.value().Get_Value()->GetAssetAs<SpriteAsset>()->_Base;
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
	

	Delegate<Unique_ptr<Texture>> Func = [path = path,lib]()
	{
		GameFiles* f = GameFiles::Get(lib);

		auto teptex =new Texture(f->ReadGameFileAsBytes(path).AsView());
		
		return Unique_ptr<Texture>(teptex);
	};
	

	return threads->AddTask_t(TaskType::File_Input,
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

	Delegate<Unique_ptr<Texture>,Unique_ptr<Texture>> Func2 = [](Unique_ptr<Texture>&& Tex) mutable
	{
		Tex->InitTexture();

		return std::move(Tex);
	};


	return threads->AddTask_t(TaskType::DataProcessing, std::move(Func), {})
		.ContinueOnThread<Unique_ptr<Texture>>(TaskType::Rendering, std::move(Func2));
}
CoreEnd