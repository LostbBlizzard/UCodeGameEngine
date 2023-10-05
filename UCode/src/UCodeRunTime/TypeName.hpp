#pragma once
#include "UCodeRunTime/Core/CoreNamespace.hpp"
#include "RunTimeBasicTypes.hpp"
CoreStart


template<typename T>
inline const char* GetTypeName()
{
	const T* Tep = nullptr;//unsed.
	return _GetTypeName<T>(Tep);
}

template<typename T> inline const char* _GetTypeName(const T* ParForOverloading)
{
	return typeid(T).name();
}
template<> inline const char* _GetTypeName<u8>(const u8* ParForOverloading) { return "uInt8"; }
template<> inline const char* _GetTypeName<u16>(const u16* ParForOverloading) { return "uInt16"; }
template<> inline const char* _GetTypeName<u32>(const u32* ParForOverloading) { return "uInt32"; }
template<> inline const char* _GetTypeName<u64>(const u64* ParForOverloading) { return "uInt64"; }

template<> inline const char* _GetTypeName<i8>(const i8* ParForOverloading) { return "Int8"; }
template<> inline const char* _GetTypeName<i16>(const i16* ParForOverloading) { return "Int16"; }
template<> inline const char* _GetTypeName<i32>(const i32* ParForOverloading) { return "Int32"; }
template<> inline const char* _GetTypeName<i64>(const i64* ParForOverloading) { return "Int64"; }

template<> inline const char* _GetTypeName<bool>(const bool* ParForOverloading) { return "bool"; }
template<> inline const char* _GetTypeName<char>(const char* ParForOverloading) { return "char"; }

/*
template<> const char* GetTypeName<char8_t>() { return "char(UTF-8)"; }
template<> const char* GetTypeName<char16_t>() { return "char(UTF-16)"; }
template<> const char* GetTypeName<char32_t>() { return "char(UTF-32)"; }
*/

template<> inline const char* _GetTypeName<String>(const String* ParForOverloading)
{
	return "String";
}

template<> inline const char* _GetTypeName<StringView>(const StringView* ParForOverloading)
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
template<typename T>inline const char* _GetTypeName(const optional<T>* ParForOverloading)
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