#include "UModule.hpp"
#include "UCodeRunTime/Modules/CoreModule.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/AssetManager.hpp"
CoreStart
void UModules::Init()
{
	if (_Modules.size() == 0)
	{
		_Modules.push_back(Unique_ptr<UModule>(new CoreModule()));
	}
}

const Span<UModule*> UModules::GetModules()
{
	Init();
	return { (UModule**)_Modules.data(),_Modules.size() };
}
UModuleComponentData* UModules::GetComponet(const UComponentsID& ID)
{
	auto List = GetModules();
	for (size_t i = 0; i < List.Size(); i++)
	{
		auto& Item = List[i];
		auto V = Item->GetComponet(ID);
		if (V)
		{
			return V;
		}

	}
	return nullptr;
}
UModuleAssetData* UModules::GetAsset(const Path& ExtWithDot)
{
	auto List = GetModules();
	for (size_t i = 0; i < List.Size(); i++)
	{
		auto& Item = List[i];
		auto V = Item->GetAssetData(ExtWithDot);
		if (V)
		{
			return V;
		}

	}
	return nullptr;
}
UModuleComponentData* UModule::GetComponet(const UComponentsID& ID)
{
	auto List = GetCompoents();
	for (size_t i = 0; i < List.Size(); i++)
	{
		auto& Item = List[i];

		if (Item.TypeData->_Type == ID)
		{
			return &Item;
		}
	}
	return nullptr;
}
UModuleAssetData* UModule::GetAssetData(const Path& ExtWithDot)
{
	auto List = GetAssets();
	for (size_t i = 0; i < List.Size(); i++)
	{
		auto& Item = List[i];

		if (Item._AssetExtWithDot == ExtWithDot)
		{
			return &Item;
		}
	}
	return nullptr;
}
CoreEnd

