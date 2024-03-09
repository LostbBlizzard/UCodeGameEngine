#pragma once
#include "../Core/GameRunTime.hpp"
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
CoreStart
class SystemList :private System
{
public:
	using CompoentKey_t = size_t;
	static SystemList* Get(Gamelibrary* lib);
	static SystemList* Find(const Gamelibrary* lib);


private:
	SystemList(Gamelibrary* lib);
	~SystemList();
	UnorderedMap<CompoentKey_t, System*> _Books;
};
CoreEnd
