#include "AssetRendering.hpp"
#include <UCodeRunTime/CoreBooks/BookOfThreads.hpp>

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
#ifdef DEBUG
	_Data.madeby = Data.madeby;
#endif // DEBUG
	runtime->DrawQuad2d(_Data);
	
}
AsynTask_t<Unique_ptr<Texture>> AssetRendering::LoadTextureAsync(Gamelibrary* lib, const Path& path)
{
	BookOfThreads* threads = BookOfThreads::Get(lib);

	Delegate<Unique_ptr<Texture>> Func = [path = path]()
	{
		auto teptex =new Texture(path);
		
		return Unique_ptr<Texture>(teptex);
	};
	

	return threads->AddTask_t(TaskType::File_Input,
		Func);
}
AsynTask_t<Unique_ptr<Texture>> AssetRendering::LoadTextureAsync(Gamelibrary* lib, const BytesView bits)
{
	BookOfThreads* threads = BookOfThreads::Get(lib);

	Unique_Bytes NewBits;
	NewBits.Copyfrom(bits);
	Vector<Byte> Bits = NewBits.MoveToVector();
	Delegate<Unique_ptr<Texture>> Func = [Bits = std::move(Bits)]()
	{
		auto View = BytesView::Make(Bits.data(), Bits.size());
		auto teptex = new Texture(View);

		return Unique_ptr<Texture>(teptex);

	};


	return threads->AddTask_t(TaskType::File_Input,
		std::move(Func));
}
CoreEnd