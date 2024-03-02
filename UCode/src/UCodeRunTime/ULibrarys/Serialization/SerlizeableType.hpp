#pragma once
#include "USerializer.hpp"


#define MakeSerlizeType(Type,Members) \
CoreStart \
template<> \
struct BitData< ##Type > { \
struct TemStruct2 \
{ \
   const Type* _this; \
	UCode::BitMaker* out =nullptr;\
	\
    template<typename T> \
	void Field(const char* field, T& v) \
	{ \
      out->WriteType(v); \
	} \
	void func() \
	{ \
      Members \
	} \
}; \
static void ToBytes(UCode::BitMaker& bit, const Type & Item) \
{ \
\
TemStruct2 v; \
v._this =&Item;\
v.out = &bit; \
v.func(); \
\
} \
struct TemStruct \
{ \
    Type* _this; \
	UCode::BitReader* out =nullptr;\
	\
    template<typename T> \
	void Field(const char* field, T& v) \
	{ \
      out->ReadType(v); \
	} \
	void func() \
	{ \
      Members \
	} \
}; \
static void FromBytes(UCode::BitReader& bit, Type & Out) \
{ \
\
TemStruct v; \
v._this =&Out;\
v.out = &bit; \
v.func(); \
} \
}; \
CoreEnd \
\
namespace YAML \
{\
	template<> struct convert< Type > { \
	struct TemStruct \
	{ \
		const Type* _this; \
		Node* out =nullptr;\
		\
		template<typename T> \
		void Field(const char* field,const T& v) \
		{ \
			(*out)[field] = v; \
		} \
		void func() \
		{ \
		Members \
		} \
	}; struct TemStruct2 \
	{ \
		Type* _this; \
		const Node* out =nullptr;\
		size_t index =0; \
		\
		template<typename T> \
		void Field(const char* field, T& v) \
		{ \
			v = (*out)[field].as<T>(v); \
			index++; \
		} \
		void func() \
		{ \
		Members \
		} \
	}; \
		static Node encode(const Type & rhs) { \
			Node node; \
			TemStruct V; \
			V._this =&rhs; \
			V.out = &node; \
			V.func(); \
			return node; \
		} \
		static bool decode(const Node& node, Type & rhs) { \
			TemStruct2 V; \
			V.out = &node; \
			V._this = &rhs;\
			V.func(); \
			return true; \
		} \
	}; \
} \

