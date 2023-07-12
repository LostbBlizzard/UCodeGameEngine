#pragma once
#include "Core/UModule.hpp"
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


private:
	Array< UModuleComponentData, 3> _Compoents;
};
CoreEnd

