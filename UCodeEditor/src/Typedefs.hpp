#pragma once
#include "Editor/EditorNamespace.hpp"
#include "UCodeRunTime/BasicTypes.hpp"
#include "UCodeRunTime/ULibrarys/AssetManagement/UID.hpp"
#include <UCodeRunTime/BasicTypes/ManagedPtr.hpp>
#include <UCodeRunTime/BasicTypes/Version.hpp>
EditorStart


namespace UC = UCode;

using Path = UC::Path;
using PathSpan = UC::PathSpan;
using PathChar = UC::PathChar;
using PathString = UC::PathString;


using String = UC::String;
using StringView = UC::StringView;

using String8 = UC::String8;
using String8View = UC::String8View;

using String16 = UC::String16;
using String16View = UC::String16View;

using String32 = UC::String32;
using String32View = UC::String32View;


template<typename T>
using Vector = UC::Vector<T>;

template<typename T>
using Span = UC::Span<T>;
	
template<typename T, size_t Size>
using Array = UC::Array<T, Size>;

template<typename T>
using Ref = UC::Ref<T>;


template<typename T>
using Unique_ptr = UC::Unique_ptr<T>;

template<typename T>
using Unique_array = UC::Unique_array<T>;


template<typename T> using Weak_ptr = UC::Weak_ptr<T>;

template<typename Key,typename U>
using Unordered_map = UC::UnorderedMap<Key,U>;

template<typename T> using Optional = UC::Optional<T>;

template<typename T> using OptionalRef = UC::OptionalRef<T>;

template<typename T> using Span = UC::Span<T>;

template<typename T> using Unique_Span = UC::Unique_Span<T>;

using BytesView = UC::BytesView;
using Unique_Bytes = UC::Unique_Bytes;

template<typename R, typename... Pars>
using Delegate = UC::Delegate<R, Pars...>;

using UID  = UC::UID;

using u8 = UC::u8;
using u16 = UC::u16;
using u32 = UC::u32;
using u64 = UC::u64;

using i8 = UC::i8;
using i16 = UC::i16;
using i32 = UC::i32;
using i64 = UC::i64;

//For Clarity reasons;

using Byte = UC::Byte;

using f32 = UC::f32;
using f64 = UC::f64;

using Vec2 = UC::Vec2;
using Vec2i = UC::Vec2i;
using Vec3 = UC::Vec3;
using Vec3i = UC::Vec3i;

template<typename T>
using Vec2_t = UC::Vec2_t<T>;

template<typename T>
using Vec3_t = UC::Vec3_t<T>;

template<typename T>
using Vec2i_t = UC::Vec2i_t<T>;

template<typename T>
using Vec3i_t = UC::Vec3i_t<T>;

using Color24 = UC::Color24;
using Color32 = UC::Color32;

using ColorRGB = UC::ColorRGB;
using ColorRGBA = UC::ColorRGBA;

using Color = UC::Color;

using Version = UC::Version;

template<typename T>
using Optional = UC::Optional<T>;

template<typename T, typename E>
using Result = UC::Result<T,E>;

template<typename T> using ManagedPtr = UC::ManagedPtr<T>;

using AnyManagedPtr = UC::AnyManagedPtr;



template<typename... T>
using Variant = UC::Variant<T...>;



#define UCODE_EDITOR_FORCEINLINE UCodeGEForceinlne
#define UCODE_EDITOR_NODISCARD UCodeGameEngineNoDiscard


template<typename T>
Span<T> spanof(Vector<T>& base, size_t offset = 0, size_t count = -1)
{
	return UC::spanof<T>(base, offset, count);
}
template<typename T>
const Span<T> spanof(const Vector<T>& base, size_t offset = 0, size_t count = -1)
{
	return UC::spanof<T>(base, offset, count);
}
template<typename T,uintptr_t Size>
Span<T> spanof(Array<T,Size>& base, size_t offset = 0, size_t count = -1)
{
	return UC::spanof<T,Size>(base, offset, count);
}
template<typename T,uintptr_t Size>
const Span<T> spanof(const Array<T,Size>& base, size_t offset = 0, size_t count = -1)
{
	return UC::spanof<T,Size>(base, offset, count);
}
EditorEnd