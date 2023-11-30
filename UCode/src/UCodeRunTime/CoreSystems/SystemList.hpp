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
 	
	template<typename T> void _AddCompoent(System* Value)
	{
		const std::type_info& info = typeid(T);
		const CompoentKey_t key = info.hash_code();



		_Books[key] = Value;//add value
	}
	template<typename T> bool _HasCompoent()
	{
		const std::type_info& info = typeid(T);
		const CompoentKey_t key = info.hash_code();

		bool InMem = _Books.count(key); // key exists
		return InMem;
	}
	template<typename T> T* _GetCompoent()
	{
		const std::type_info& info = typeid(T);
		const CompoentKey_t key = info.hash_code();

		if (_HasCompoent<T>())
		{
			return (T*)_Books[key];
		}
		else
		{
			return nullptr;
		}
	}
	template<typename T> void _RemoveCompoent()
	{
		const std::type_info& info = typeid(T);
		const CompoentKey_t key = info.hash_code();



		_Books.erase(key);
	}
private:
	SystemList(Gamelibrary* lib);
	~SystemList();
	UnorderedMap<CompoentKey_t, System*> _Books;
};
CoreEnd
