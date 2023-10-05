#include "CMem.hpp"
#include "UCodeRunTime/UDefs.hpp"

#include <iostream>
CoreStart

namespace Mem
{
	#define DEBUG_Mem 0
	String Get_Size(size_t Size)
	{
		constexpr size_t gb = 1024 * 1024 * 1024;
		constexpr size_t mb = 1024 * 1024;
		constexpr size_t kb = 1024;
		if (Size >= gb) return std::to_string((float)Size / gb) + " GB ";
		if (Size >= mb) return std::to_string((float)Size / mb) + " MB ";
		if (Size >= kb) return std::to_string((float)Size / kb) + " KB ";
		return std::to_string(Size) + " B ";
	}

	MemProfiler::~MemProfiler()
	{
		size_t newmemSize = Get_UsedMem_thread_local();
		size_t newallocSize = Get_Allocate_thread_localValue();
		size_t newFreeSize = Get_Deallocate_thread_localValue();


		size_t MemChage = oldmemSize- newmemSize;

		size_t Allocs = newallocSize-oldAllocateSize;

		size_t FreeSize = newFreeSize - oldDeallocateSize;

		UCODE_ENGINE_LOG("New Profile is MemChage:" << Get_Size(MemChage) 
			<< " Allocs:" << Get_Size(Allocs)
		    << " Frees:" << Get_Size(FreeSize) );
	}

	MemFreeChecker::~MemFreeChecker()
	{
		size_t newmemSize = Get_UsedMem_thread_local();
		size_t newallocSize = Get_Allocate_thread_localValue();
		size_t newFreeSize = Get_Deallocate_thread_localValue();


		size_t MemChage = oldmemSize - newmemSize;

		size_t Allocs = newallocSize - oldAllocateSize;

		size_t FreeSize = newFreeSize - oldDeallocateSize;
		if (Allocs > FreeSize)
		{
			String V = Get_Size(Allocs - FreeSize);
			UCODE_ENGINE_ERROR( ((String)"MemFreeChecker:you forgot to free " + V).c_str() );
		}
	}
	static thread_local size_t threadLocalMemCount = 0;
	static thread_local size_t threadLocalAllocateCount = 0;
	static thread_local size_t threadLocalFreeCount = 0;

	size_t Get_UsedMem_thread_local()
	{
		return threadLocalMemCount;
	}
	size_t Get_Allocate_thread_localValue()
	{
		return threadLocalAllocateCount;
	}
	size_t Get_Deallocate_thread_localValue()
	{
		return threadLocalFreeCount;
	}


	void* malloc(size_t size)
	{		
		auto r = std::malloc(size + sizeof(size_t));



		if (r == nullptr)
		{
			UCodeGameEngineThrowException("malloc failed no more memory");
		}
		
		size_t* r2 = (size_t*)r;
		*r2 = size;
		
		#if DEBUG_Mem
		r2++;
		threadLocalAllocateCount += size;
		
		//std::cout << "Malloc:" << size << "\n";
		#endif // DEBUG_Mem

		return r2;
	}

	void* calloc(size_t num, size_t size)
	{
		UCodeGameEngineThrowException("not added");
		auto r = std::calloc(num, size);
		if (r == nullptr)
		{
			UCodeGameEngineThrowException("calloc failed no more memory");
		}
		return r;
	}

	void* realloc(void* _Block, size_t _Size)
	{
		UCodeGameEngineThrowException("not added");

		auto r = std::realloc(_Block, _Size);
		if (r == nullptr)
		{
			UCodeGameEngineThrowException("realloc failed no more memory");
		}
		return r;
	}

	void free(void* _Block)
	{
		size_t* r2 = (size_t*)_Block;

		#if DEBUG_Mem
		r2--;

		size_t BlockSize = *r2;
		threadLocalMemCount -= BlockSize;
		threadLocalFreeCount += BlockSize;

		//std::cout << "Free:" << BlockSize << "\n";
		#endif // DEBUG_Mem

		std::free(r2);
	}

	

}
CoreEnd

void* operator new(size_t size)
{
	return  Malloc(size);
}
void operator delete(void* p)
{
	return Free(p);
}