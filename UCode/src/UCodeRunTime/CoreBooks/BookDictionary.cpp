#include "BookDictionary.hpp"
#include <UCodeRunTime/StaticComponentsIndex.hpp>
CoreStart

constexpr StaticBooksIndex_t KeyIdex = (StaticBooksIndex_t)StaticBooksIndex::Dictionary;

BockDictionary::BockDictionary(Gamelibrary* lib) :libraryBook(lib)
{
	if (lib->Get_StaticComponent(KeyIdex)) { Destroy(this); return; }
	lib->SetStaticComponent(KeyIdex, this);
}
BockDictionary::~BockDictionary()
{
}

BockDictionary* BockDictionary::Get(Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	if (V) { return (BockDictionary*)V; }
	
	BockDictionary* r = new BockDictionary(lib);;

	lib->MoveBook(r);
	lib->SetStaticComponent(KeyIdex, r);
	return r;
}
BockDictionary* BockDictionary::Find(const Gamelibrary* lib)
{
	auto V = lib->Get_StaticComponent(KeyIdex);
	return (BockDictionary*)V; 
}
CoreEnd
