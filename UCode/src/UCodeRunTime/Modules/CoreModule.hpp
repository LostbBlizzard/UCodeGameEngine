#pragma once
#include "../Core/UModule.hpp"
CoreStart
class CoreModule :public UModule
{
public:
	CoreModule();
	~CoreModule();
	const Span<UModuleComponentData> GetCompoents() override 
	{
		return { _Compoents.data(),_Compoents.size() };
	};
	
	const Span<UModuleAssetData> GetAssets() override
	{
		return { _Assets.data(),_Assets.size() };
	};

private:
	Array< UModuleComponentData, 3> _Compoents;
	Array<UModuleAssetData,2> _Assets;
};
CoreEnd

