#pragma once
#include "../Core/CoreNamespace.hpp"
#include "../UDefs.hpp"
#include <functional>//hash
#include <iterator>
CoreStart


template<typename T>
struct NullablePtr;

template<typename T> 
struct NeverNullPtr
{
public:
	using PtrType = T*;
	using ThisType = NeverNullPtr<T>;
	using NullableType= NullablePtr<T>;

	NeverNullPtr()
		:Value(nullptr)
	{

	}
	NeverNullPtr(PtrType Value)
		:Value(Value)
	{	
		UCodeGEAssert(Value);//never null
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne static const ThisType Make(const T* Value)
	{
		return ThisType((PtrType)Value);
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne static ThisType Make(T* Value)
	{
		return ThisType(Value);
	}
	
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr PtrType value() noexcept
	{
		return Value;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr const PtrType value() const noexcept
	{
		return Value;
	}

	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr const T* operator->() const noexcept
	{
		return Value;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr T* operator->() noexcept
	{
		return Value;
	}

	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr const T& operator*() const noexcept
	{
		return *Value;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr T& operator*() noexcept
	{
		return *Value;
	}

	UCodeGameEngineNoDiscard UCodeGEForceinlne bool operator==(const NeverNullPtr other) const
	{
		return Value == other.Value;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne bool operator!=(const NeverNullPtr other) const
	{
		return Value != other.Value;
	}


	UCodeGameEngineNoDiscard UCodeGEForceinlne const NullableType AsNullable() const
	{
		const NullableType r = NullableType::Make(value());
		return r;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne NullableType AsNullable()
	{
		NullableType r = NullableType::Make(value());
		return r;
	}
private:
	PtrType Value;
};

template<typename T> UCodeGameEngineNoDiscard NeverNullPtr<T> NeverNullptr(T* Value)
{
	return NeverNullPtr<T>::Make(Value);
}

template<typename T> UCodeGameEngineNoDiscard const NeverNullPtr<T> NeverNullptr(const T* Value)
{
	return NeverNullPtr<T>::Make(Value);
}




template<typename T> 
struct NullablePtr
{
public:
	using PtrType = T*;
	using ThisType = NullablePtr<T>;
	using NeverNullType = NeverNullPtr<T>;
	NullablePtr()
		:Value(nullptr)
	{

	}
	NullablePtr(PtrType Value)
		:Value(Value)
	{

	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne static ThisType Make(T* Value)
	{
		return ThisType(Value);
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne static const ThisType Make(const T* Value)
	{
		return ThisType((PtrType)Value);
	}

	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr explicit operator bool() const
	{
		return Value != nullptr;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr bool has_value() const noexcept
	{
		return Value != nullptr;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr const NeverNullType value() const
	{
		UCodeGEAssert(Value);
		return NeverNullType::Make(Value);
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr NeverNullType value()
	{
		UCodeGEAssert(Value);
		return NeverNullType::Make(Value);
	}


	
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr PtrType value_unchecked()noexcept
	{
		return Value;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne constexpr const PtrType value_unchecked() const noexcept
	{
		return Value;
	}

	UCodeGameEngineNoDiscard UCodeGEForceinlne NeverNullType value_or(NeverNullType other)
	{
		if (has_value())
		{
			return value();
		}
		else
		{
			return other;
		}
	}

	UCodeGameEngineNoDiscard UCodeGEForceinlne const NeverNullType value_or(NeverNullType other) const
	{
		if (has_value())
		{
			return value();
		}
		else
		{
			return other;
		}
	}

	UCodeGameEngineNoDiscard UCodeGEForceinlne bool operator==(const NullablePtr other) const
	{
		return Value == other.Value;
	}
	UCodeGameEngineNoDiscard UCodeGEForceinlne bool operator!=(const NullablePtr other) const
	{
		return Value == other.Value;
	}
private:
	PtrType Value;
};




template<typename T> UCodeGameEngineNoDiscard NullablePtr<T> Nullableptr(T* Value)
{
	return NullablePtr<T>::Make(Value);
}
template<typename T> UCodeGameEngineNoDiscard const NullablePtr<T> Nullableptr(const T* Value)
{
	return NullablePtr<T>::Make(Value);
}



template<typename T>
struct OptionalRef
{
public:
	using NeverNullType = NeverNullPtr<T>;
	using NullableType = NullablePtr<T>;

	constexpr OptionalRef()
	{

	}
	explicit OptionalRef(T& value)
		: base(&value)
	{

	}
	NullableType asNullable()
	{
		return NullableType::Make(base);
	}
	const NullableType asNullable() const
	{
		return NullableType::Make(base);
	}

	constexpr bool has_value() const noexcept
	{
		return base != nullptr;
	}

	const T& value() const
	{
		UCodeGEAssert(base);
		return *base;
	}
	T& value() 
	{
		UCodeGEAssert(base);
		return *base;
	}
private:
	T* base =nullptr;
};

template<typename T> OptionalRef<T> Optionalref(T& Value)
{
	return OptionalRef(Value);
}
template<typename T> const OptionalRef<T> Optionalref(const T& Value)
{
	return OptionalRef((T&)Value);
}

CoreEnd

template<typename T>
struct std::hash<UCode::NullablePtr<T>>
{
	std::size_t operator()(const UCode::NullablePtr<T>& k) const
	{
		auto hasher = hash<T*>();

		return hasher(k.value_unchecked());
	}
};

template<typename T>
struct std::hash<UCode::NeverNullPtr<T>>
{
	std::size_t operator()(const UCode::NeverNullPtr<T>& k) const
	{
		auto hasher = hash<T*>();

		return hasher(k.value());
	}
};
