#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "UCodeRunTime/UDefs.hpp"
#include "Ref.hpp"
CoreStart


template<typename T> 
struct Span
{
	constexpr Span() :_Data(nullptr), _Size(0) {}
	constexpr Span(T* data,size_t Size): _Data(data),_Size(Size){}

	constexpr UCODE_ENGINE_FORCE_INLINE T& operator[](size_t Index)
	{
		#ifdef DEBUG
		if (Index > _Size)
		{
			throw std::exception("Index out of range");
		}
		#endif // DEBUG

		return _Data[Index];
	}

	constexpr UCODE_ENGINE_FORCE_INLINE const T& operator[](size_t Index) const
	{
		#ifdef DEBUG
		if (Index > _Size)
		{
			throw std::exception("Index out of range");
		}
		#endif // DEBUG

		return _Data[Index];
	}	
	
	
	constexpr UCODE_ENGINE_FORCE_INLINE T* Data()const
	{
		return _Data;
	}
	constexpr UCODE_ENGINE_FORCE_INLINE size_t Size()const
	{
		return _Size;
	}

	constexpr UCODE_ENGINE_FORCE_INLINE static Span Make(T* data, size_t Size)
	{
		return Span(data, Size);
	}
	constexpr UCODE_ENGINE_FORCE_INLINE static const Span Make(const T* data, size_t Size)
	{
		return Span((T*)data, Size);
	}
private:
	T* _Data;
	size_t _Size;
};


template<typename T>
struct Unique_Span
{
	using Span_t = Span<T>;

	Unique_Span() :Pointer(nullptr), Size(0)
	{

	}
	Unique_array<T> Pointer;
	size_t Size;


	constexpr UCODE_ENGINE_FORCE_INLINE T& operator[](size_t Index)
	{
		#ifdef DEBUG
		if (Index > _Size)
		{
			throw std::exception("Index out of range");
		}
		#endif // DEBUG

		return Pointer[Index];
	}

	constexpr UCODE_ENGINE_FORCE_INLINE const T& operator[](size_t Index) const
	{
		#ifdef DEBUG
		if (Index > _Size)
		{
			throw std::exception("Index out of range");
		}
		#endif // DEBUG

		return Pointer[Index];
	}

	const Span_t AsView() const
	{
		return Span_t(Pointer.get(), Size);
	}
	Span_t AsView()
	{
		return Span_t(Pointer.get(), Size);
	}
	void Resize(size_t Count)
	{
		Pointer.reset(new T[Count]);
		NewBits.Size = Count;
	}
	void Copyfrom(const Span_t& Values)
	{
		Resize(Values.Size());
		for (size_t i = 0; i < Values.Size(); i++)
		{
			Pointer[i] = Values[i];
		}
	}
	void Copyfrom(Span_t&& Values)
	{
		Resize(Values.Size());
		for (size_t i = 0; i < Values.Size(); i++)
		{
			Pointer[i] = std::move(Values[i]);
		}
	}
};

using BytesView = Span<Byte>;
using Unique_Bytes = Unique_Span<Byte>;

CoreEnd