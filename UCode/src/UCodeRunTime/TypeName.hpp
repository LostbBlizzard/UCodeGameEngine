#pragma once
#include "UCodeRunTime/Core/CoreNamespace.hpp"
#include "BasicTypes.hpp"
CoreStart

template<typename T>
inline const char* GetTypeName();

template<typename T> inline const char* _GetTypeName(const T* ParForOverloading)
{
	return typeid(T).name();
}
 inline const char* _GetTypeName(const u8* ParForOverloading) { return "uInt8"; }
 inline const char* _GetTypeName(const u16* ParForOverloading) { return "uInt16"; }
 inline const char* _GetTypeName(const u32* ParForOverloading) { return "uInt32"; }
 inline const char* _GetTypeName(const u64* ParForOverloading) { return "uInt64"; }

 inline const char* _GetTypeName(const i8* ParForOverloading) { return "Int8"; }
 inline const char* _GetTypeName(const i16* ParForOverloading) { return "Int16"; }
 inline const char* _GetTypeName(const i32* ParForOverloading) { return "Int32"; }
 inline const char* _GetTypeName(const i64* ParForOverloading) { return "Int64"; }

 inline const char* _GetTypeName(const bool* ParForOverloading) { return "bool"; }
 inline const char* _GetTypeName(const char* ParForOverloading) { return "char"; }

/*
 const char* GetTypeName<char8_t>() { return "char(UTF-8)"; }
 const char* GetTypeName<char16_t>() { return "char(UTF-16)"; }
 const char* GetTypeName<char32_t>() { return "char(UTF-32)"; }
*/

inline const char* _GetTypeName(const String* ParForOverloading)
{
	return "String";
}

inline const char* _GetTypeName(const StringView* ParForOverloading)
{
	return "StringView";
}

template<typename T>inline const char* _GetTypeName(const Vector<T>* ParForOverloading)
{
	static const String Buffer = GetTypeName<T>() + "[]";
	return Buffer.c_str();
}

template<typename T, size_t Size>inline const char* _GetTypeName(const Array<T,Size>* ParForOverloading)
{
	static const String Buffer = GetTypeName<T>() + "[" + std::to_string(Size) + "]";
	return Buffer.c_str();
}

template<typename T> inline const char* _GetTypeName(const Span<T>* ParForOverloading)
{
	static const String Buffer = GetTypeName<T>() + "[:]";
	return Buffer.c_str();
}
template<typename T>inline const char* _GetTypeName(const Optional<T>* ParForOverloading)
{
	static const String Buffer = GetTypeName<T>() + "?";
	return Buffer.c_str();
}
template<typename T>inline const char* _GetTypeName(const Unique_ptr<T>* ParForOverloading)
{
	static const String Buffer = GetTypeName<T>() + "^";
	return Buffer.c_str();
}

template<typename T>inline const char* _GetTypeName(const Ref<T>* ParForOverloading)
{
	static const String Buffer = GetTypeName<T>() + "$";
	return Buffer.c_str();
}

template<typename T>
inline const char* GetTypeName()
{
	const T* Tep = nullptr;//unsed.
	return _GetTypeName<T>(Tep);
}



template <typename... Types>
struct NameVariant_helper;

template <typename T2, typename... Types2>
struct NameVariant_helper<T2, Types2...>
{
	UCodeGameEngineForceinlne static const char* GetName(const UCode::VariantTagType type_index)
	{
		if (type_index == sizeof...(Types2))
		{
			return GetTypeName<T2>();
		}
		else
		{
			return NameVariant_helper<Types2...>::GetName(type_index);
		}
	}
};
template <>
struct NameVariant_helper<>
{
	UCodeGameEngineForceinlne static const char* GetName(const UCode::VariantTagType type_index) { return {}; }
};

template<typename... Types> inline const char*  _GetTypeName(const Variant<Types...>* ParForOverloading)
{
	using Type = Variant<Types...>;
	using NameVariant = NameVariant_helper<Types...>;
	

	static const String Buffer = []()
	{
		String V;
		V += "[";

		for (size_t i = 0; i < Type::ItemsInListCount; i++)
		{
			const char* Str = NameVariant::GetName(i);
			V += Str;

			if (i + 1 < Type::ItemsInListCount)
			{
				V += ",";
			}
		}

		V += "]";
		return V;
	}();
	return Buffer.c_str();
}
CoreEnd