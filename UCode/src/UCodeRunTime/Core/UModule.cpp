#include "UModule.hpp"
#include "../CoreModule.hpp"
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

CoreEnd

