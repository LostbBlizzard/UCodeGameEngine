#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "UCodeRunTime/UDefs.hpp"
#include "Ref.hpp"
#include "Vector.hpp"
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
	
	constexpr UCODE_ENGINE_FORCE_INLINE const T* Data()const
	{
		return _Data;
	}
	constexpr UCODE_ENGINE_FORCE_INLINE T* Data()
	{
		return _Data;
	}
	constexpr UCODE_ENGINE_FORCE_INLINE size_t Size()const
	{
		return _Size;
	}

	constexpr UCODE_ENGINE_FORCE_INLINE static Span<T> Make(T* data, size_t Size)
	{
		return Span(data, Size);
	}
	constexpr UCODE_ENGINE_FORCE_INLINE static const Span<T> Make(const T* data, size_t Size)
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
	using This = Unique_Span<T>;
	using Span_t = Span<T>;

	Unique_Span() :Pointer(nullptr), _Size(0)
	{

	}


	Unique_Span(This&& Other) :Pointer(std::move(Other.Pointer)), _Size(Other._Size)
	{
		Other._Size = 0;
	}
	This& operator=(This&& Other)
	{
		Pointer = std::move(Other.Pointer);
		_Size = Other._Size;

		Other._Size = 0;
		return *this;
	}

	constexpr UCODE_ENGINE_FORCE_INLINE T& operator[](size_t Index)
	{
		#ifdef DEBUG
		if (Index >= _Size)
		{
			throw std::exception("Index out of range");
		}
		#endif // DEBUG

		return Pointer[Index];
	}

	constexpr UCODE_ENGINE_FORCE_INLINE const T& operator[](size_t Index) const
	{
		#ifdef DEBUG
		if (Index >= _Size)
		{
			throw std::exception("Index out of range");
		}
		#endif // DEBUG

		return Pointer[Index];
	}

	const Span_t AsView() const
	{
		return Span_t::Make(Pointer.get(), _Size);
	}
	Span_t AsView()
	{
		return Span_t::Make(Pointer.get(), _Size);
	}
	void Resize(size_t Count)
	{
		Pointer.reset(new T[Count]);
		_Size = Count;
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

		Values._Size = 0;
	}

	void Copyfrom(const Vector<T>& Values)
	{
		Resize(Values.Size());
		for (size_t i = 0; i < Values.Size(); i++)
		{
			Pointer[i] = Values[i];
		}
	}
	void Copyfrom(Vector<T>&& Values)
	{
		Resize(Values.size());
		for (size_t i = 0; i < Values.size(); i++)
		{
			Pointer[i] = std::move(Values[i]);
		}

		Values.resize(0);
	}


	Vector<T> ToVector() const
	{
		Vector<T> R;
		R.resize(_Size);

		for (size_t i = 0; i < _Size; i++)
		{
			R[i] = this->operator[](i);
		}

		return R;
	}
	Vector<T> MoveToVector() 
	{
		Vector<T> R;
		R.resize(_Size);

		for (size_t i = 0; i < _Size; i++)
		{
			R[i] = std::move(this->operator[](i));
		}

		_Size = 0;

		return R;
	}

	size_t Size() const
	{
		return _Size;
	}
	T* Data()
	{
		return Pointer.get();
	}

	const T* Data() const
	{
		return Pointer.get();
	}
private:
		Unique_array<T> Pointer;
		size_t _Size;
};

using BytesView = Span<Byte>;
using Unique_Bytes = Unique_Span<Byte>;

CoreEnd