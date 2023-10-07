#pragma once
#include "Editor/EditorNamespace.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/UID.hpp"
#include <UCodeRunTime/BasicTypes/ManagedPtr.hpp>
EditorStart



using Path = UCode::Path;
using PathSpan = UCode::PathSpan;
using PathChar = UCode::PathChar;
using PathString = UCode::PathString;


using String = UCode::String;
using StringView = UCode::StringView;

using String8 = UCode::String8;
using String8View = UCode::String8View;

using String16 = UCode::String16;
using String16View = UCode::String16View;

using String32 = UCode::String32;
using String32View = UCode::String32View;


template<typename T>
using Vector = UCode::Vector<T>;

template<typename T>
using Span = UCode::Span<T>;

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
using Unordered_map = UCode::UnorderedMap<Key,U>;

template<typename T> using Optional = UCode::Optional<T>;

template<typename T> using Span = UCode::Span<T>;

template<typename T> using Unique_Span = UCode::Unique_Span<T>;

using BytesView = UCode::BytesView;
using Unique_Bytes = UCode::Unique_Bytes;

template<typename R, typename... Pars>
using Delegate = UCode::Delegate<R, Pars...>;

using UID  = UCode::UID;

using u8 = UCode::u8;
using u16 = UCode::u16;
using u32 = UCode::u32;
using u64 = UCode::u64;

using i8 = UCode::i8;
using i16 = UCode::i16;
using i32 = UCode::i32;
using i64 = UCode::i64;

//For Clarity reasons;

using Byte = UCode::Byte;

using f32 = UCode::f32;
using f64 = UCode::f64;

using Vec2 = UCode::Vec2;
using Vec2i = UCode::Vec2i;
using Vec3 = UCode::Vec3;
using Vec3i = UCode::Vec3i;
using Version = UCode::Version;

template<typename T>
using Optional = UCode::Optional<T>;

template<typename T, typename E>
using Result = UCode::Result<T,E>;

template<typename T> using ManagedPtr = UCode::ManagedPtr<T>;

using AnyManagedPtr = UCode::AnyManagedPtr;



template<typename... T>
using Variant = UCode::Variant<T...>;



#define UCODE_EDITOR_FORCEINLINE UCodeGameEngineForceinlne
#define UCODE_EDITOR_NODISCARD UCodeGameEngineNoDiscard


EditorEnd