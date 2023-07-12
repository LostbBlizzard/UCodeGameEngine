#pragma once
#include "UCodeRunTime/RunTimeBasicTypes.hpp"
#include "Editor/EditorNamespace.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/UID.hpp"
#include <UCodeRunTime/RunTimeBasicTypes/Delegate.hpp>
#include <UCodeRunTime/RunTimeBasicTypes/ManagedPtr.hpp>
#include <UCodeLang/LangCore/DataType/BinaryVectorMap.hpp>
EditorStart

using String = UCode::String;
using Path = UCode::Path;
using StringView = UCode::StringView;

template<typename T>
using Vector = UCode::Vector<T>;

template<typename T, size_t Size>
using Array = UCode::Array<T, Size>;

template<typename T>
using Ref = UCode::Ref<T>;


template<typename T>
using Unique_ptr = UCode::Unique_ptr<T>;

template<typename T>
using Unique_array = UCode::Unique_array<T>;


template<typename T> using Weak_ptr = UCode::Weak_ptr<T>;

template<typename Key,typename U>
using Unordered_map = UCode::Unordered_map<Key,U>;

template<typename T> using optional = UCode::optional<T>;

template<typename T> using Span = UCode::Span<T>;

template<typename T> using Unique_Span = UCode::Unique_Span<T>;

using BytesView = UCode::BytesView;
using Unique_Bytes = UCode::Unique_Bytes;

template<typename R, typename... Pars>
using Delegate = UCode::Delegate<R, Pars...>;

typedef UCode::UID UID;

typedef UCode::UInt8 UInt8;
typedef UCode::UInt16 UInt16;
typedef UCode::UInt32 UInt32;
typedef UCode::UInt64 UInt64;


typedef UCode::SInt8 SInt8;
typedef UCode::SInt16 SInt16;
typedef UCode::SInt32 SInt32;
typedef UCode::SInt64 SInt64;

typedef UCode::float32 float32;
typedef UCode::float64 float64;


//For Clarity reasons
typedef  UCode::Byte Byte;

typedef UCode::Vec2 Vec2;
typedef UCode::Vec2i Vec2i;
typedef UCode::Vec3 Vec3;
typedef UCode::Vec3i Vec3i;

typedef UCode::Version Version;

template<typename T> using ManagedPtr = UCode::ManagedPtr<T>;

using AnyManagedPtr = UCode::AnyManagedPtr;



template<typename... T>
using Variant = UCode::Variant<T...>;

template<typename Key, typename U>
using BinaryVectorMap = UCodeLang::BinaryVectorMap<Key,U>;

template<typename Key, typename U>
using VectorMap = UCodeLang::VectorMap<Key, U>;

#define UCODE_EDITOR_FORCEINLINE UCODE_ENGINE_FORCE_INLINE
#define UCODE_EDITOR_NODISCARD UCODE_ENGINE_NODISCARD


EditorEnd