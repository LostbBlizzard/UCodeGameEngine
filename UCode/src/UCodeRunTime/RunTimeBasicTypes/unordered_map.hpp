#pragma once
#include <unordered_map>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeLang/LangCore/DataType/BinaryVectorMap.hpp>
CoreStart
template<typename Key,typename T>
using Unordered_map = std::unordered_map<Key,T>;////Im probably not going to add my own implementation.this is here for consistency.

template<typename Key, typename U>
using BinaryVectorMap = UCodeLang::BinaryVectorMap<Key, U>;

template<typename Key, typename U>
using VectorMap = UCodeLang::VectorMap<Key, U>;
CoreEnd