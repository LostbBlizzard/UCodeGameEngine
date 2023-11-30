#include "SystemList.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart

constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::Dictionary;

SystemList::SystemList(Gamelibrary* lib) :System(lib)
{
	if (lib->Get_StaticComponent(KeyIdex)) { Destroy(this); return; }
	lib->SetStaticComponent(KeyIdex, this);
}
SystemList::~SystemList()
{
}

SystemList* SystemList::Get(Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	if (V) { return (SystemList*)V; }
	
	SystemList* r = new SystemList(lib);;

	lib->MoveSystem(r);
	lib->SetStaticComponent(KeyIdex, r);
	return r;
}
SystemList* SystemList::Find(const Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	return (SystemList*)V; 
}
CoreEnd
