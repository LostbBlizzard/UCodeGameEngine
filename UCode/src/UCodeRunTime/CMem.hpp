#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <memory>
#include <new>     



CoreStart

namespace Mem 
{
	

	size_t Get_UsedMem_thread_local();
	size_t Get_Allocate_thread_localValue();
	size_t Get_Deallocate_thread_localValue();


	void* malloc(size_t size);;
	void* calloc(size_t num, size_t size);
	void* realloc(void* _Block, size_t _Size);
	void free(void* _Block);
	
	struct MemProfiler
	{
		const size_t oldmemSize =Get_UsedMem_thread_local();
		const size_t oldAllocateSize = Get_Allocate_thread_localValue();
		const size_t oldDeallocateSize = Get_Deallocate_thread_localValue();
		~MemProfiler();
	};

	struct MemFreeChecker
	{
		const size_t oldmemSize = Get_UsedMem_thread_local();
		const size_t oldAllocateSize = Get_Allocate_thread_localValue();
		const size_t oldDeallocateSize = Get_Deallocate_thread_localValue();
		~MemFreeChecker();
	};
}

CoreEnd

inline static void* Malloc(size_t size)
{
	return Core::Mem::malloc(size);
}
inline static void Free(void* _Block)
{
	Core::Mem::free(_Block);
}
inline static void* Realloc(void* _Block, size_t _Size)
{
	return Core::Mem::realloc(_Block, _Size);
}

//
void* operator new(size_t size);
void operator delete(void* p);

