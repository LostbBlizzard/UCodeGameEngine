#pragma once

#ifdef DEBUG
#include <iostream>
#include <UCodeRunTime/UDefs.hpp>
#endif  
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <memory>

CoreStart
template<typename T>
struct Ref
{
	
public:
	struct RefData
	{
		T Data;
		size_t RefCount;

		template<typename... Pars>
		RefData(Pars... parameters) :Data(parameters...), RefCount(0){}
		
		~RefData() = default;
	};
	Ref() :_Data(nullptr)
	{

	}
	Ref(const T& Data) : _Data(new RefData(Data))
	{
		AddPointerRef();
	}
	Ref(T&& Data) : _Data(new RefData(Data))
	{
		AddPointerRef();
	}

	template<typename... Pars> 
	static Ref Create(Pars...  parameters)
	{
		Ref r = Ref();
		r._Data = new RefData(parameters...);
		r.AddPointerRef();
		return  r;
	}
	static Ref Move(T&& Ptr)
	{
		return  Ref(Ptr);
	}
	static Ref Bind(RefData* Ptr)
	{
		Ref r = Ref();
		r._Data = Ptr;
		r.AddPointerRef();
		return  r;
	}
	
	~Ref()
	{
		ReMovePointerRef();
	}
	
	Ref(const Ref& ToCopy) :_Data(ToCopy._Data)
	{
		AddPointerRef();
	}
	UCODE_ENGINE_FORCE_INLINE Ref& operator=(const Ref& ToCopy)
	{
		ReMovePointerRef();

		_Data = ToCopy._Data;
		AddPointerRef();

		return *this;
	}

	UCODE_ENGINE_FORCE_INLINE void SetToNull()
	{
		ReMovePointerRef();
		_Data = nullptr;
	}
	
	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE const T* get() const
	{
		if (_Data == nullptr)
		{
			return nullptr;
		}

		return &_Data->Data;
	}
	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE const T* operator->() const {return get();}
	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE const T& operator*() const { return *get(); }
	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE const T& operator[](size_t Index) const { return get()[Index]; }

	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE T* get()
	{
		if (_Data == nullptr)
		{
			return nullptr;
		}

		return &_Data->Data; 
	}
	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE T* operator->() { return get(); }
	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE T& operator*() { return *get(); }
	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE T& operator[](size_t Index) { return get()[Index]; }

	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE size_t get_RefCount() const { return _Data->RefCount; }
	UCODE_ENGINE_NODISCARD UCODE_ENGINE_FORCE_INLINE RefData* get_DatatPtr() { return _Data; }
private:
	RefData* _Data;
	void AddPointerRef()
	{
		if (_Data == nullptr){return;}
		_Data->RefCount++;
	}
	void ReMovePointerRef()
	{
		if (_Data == nullptr) { return; }
	
		_Data->RefCount--;
		if (_Data->RefCount == 0)
		{
			delete _Data;
		}	
	}
};

template<typename T> using Weak_ptr = std::weak_ptr<T>;//this need it's own implementation


template<typename T>
using Unique_ptr = std::unique_ptr<T>;

template<typename T>
using Unique_array = std::unique_ptr<T[]>;




CoreEnd
