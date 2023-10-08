#pragma once
#include "UCodeRunTime/Core/CoreNamespace.hpp"

#include <UCodeRunTime/BasicTypes.hpp>
CoreStart

struct AnyManagedPtr;

template<typename T>
struct ManagedPtr
{
public:
	using Ptr_t = T*;
	ManagedPtr() :_Ptr(nullptr)
	{

	}
	ManagedPtr(Ptr_t Value):_Ptr(Value)
	{
	
	}
	inline const auto Get_Value() const
	{
		return *_Ptr;
	}
	inline auto Get_Value()
	{
		return *_Ptr;
	}

	inline void Set_Value(Ptr_t Value)
	{
		*_Ptr.get() = Value;
	}

	inline bool Has_Value()const
	{
		return *_Ptr.get();
	}
	inline size_t GetCounter()const
	{
		return _Ptr.get_RefCount();
	}
private:
	Ref<Ptr_t> _Ptr;
};


struct AnyManagedPtr
{
	using _Any = u8;
	ManagedPtr<_Any> _This;

	AnyManagedPtr()
	{

	}
	AnyManagedPtr(void* Value):_This(nullptr)
	{

	}
	void SetValue(void* Value)
	{
		_This.Set_Value((_Any*)Value);
	}
	auto GetValue()const
	{
		return (void*)_This.Get_Value();
	}

	size_t GetCounter()const
	{
		return _This.GetCounter();
	}

	


	template<typename T>
	inline static AnyManagedPtr As(ManagedPtr<T> Ptr)
	{
		return *(AnyManagedPtr*)&Ptr;
	}

	template<typename T>
	inline static AnyManagedPtr& As_ptr(ManagedPtr<T>& Ptr)
	{
		return *(AnyManagedPtr*)&Ptr;
	}

	


	template<typename T>
	inline static ManagedPtr<T> As(AnyManagedPtr Ptr)
	{
		return *(ManagedPtr<T>*)&Ptr;
	}

	template<typename T>
	inline static ManagedPtr<T>& As_ptr(AnyManagedPtr& Ptr)
	{
		return *(ManagedPtr<T>*)&Ptr;
	}

	template<typename T> 
	inline ManagedPtr<T> As()
	{
		return As<T>(*this);
	}

	template<typename T> 
	inline ManagedPtr<T>& As_ptr()
	{
		return As_ptr<T>(*this);
	}

	~AnyManagedPtr() noexcept
	{
		UCodeGameEngineAssert(!_This.Has_Value() || (_This.Has_Value() && _This.GetCounter() == 1));
		//"Destructor not called for Managed object"
	}
};

CoreEnd
