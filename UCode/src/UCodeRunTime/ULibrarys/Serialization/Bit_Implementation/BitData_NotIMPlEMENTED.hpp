#pragma once
#define BitData_NotIMPlEMENTED(Type)\
template<> struct BitData< Type >\
{\
	using _Type = Type##;\
	static void ToBytes(BitMaker& This, const _Type& Value)\
	{\
		UCODE_ENGINE_IMPlEMENTED_LATER;\
	}\
	static void FromBytes(BitReader& This, _Type& Out)\
	{\
		UCODE_ENGINE_IMPlEMENTED_LATER;\
	}\
};\
