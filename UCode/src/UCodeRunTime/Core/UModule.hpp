#pragma once
#include "CoreNamespace.hpp"
#include "../BasicTypes.hpp"
#include "Entity.hpp"
CoreStart

struct UModuleComponentData
{
	UComponentData* TypeData=nullptr;
	UModuleComponentData(){}
	UModuleComponentData(UComponentData* V)
	{
		TypeData = V;
	}
};

class UModule
{
public:
	UModule()
	{

	}
	virtual ~UModule()
	{

	}
	virtual const Span<UModuleComponentData> GetCompoents() { return {}; };


	UModuleComponentData* GetComponet(const UComponentsID& ID);

private:
};

using UModuleInit = UModule* (*)();


class UModules
{
public:

	static void Init();
	static const Span<UModule*> GetModules();

	static UModuleComponentData* GetComponet(const UComponentsID& ID);
private:
	inline static Vector<Unique_ptr<UModule>> _Modules;
};
CoreEnd