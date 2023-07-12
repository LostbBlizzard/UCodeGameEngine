#pragma once
#include <xhash>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/ULibrarys/Others/Random.hpp>
CoreStart
struct UID
{
	UID();
	typedef UInt64 UID_t;

	constexpr bool operator==(const UID& other) const { return (_Value == other._Value); }
	constexpr bool operator!=(const UID& other) const { return !operator==(other); }

	static void MakeNew(Random& rng, UID& out);


	UCODE_ENGINE_FORCE_INLINE void Set_Value(UID_t Value) { _Value = Value; }
	UCODE_ENGINE_FORCE_INLINE auto Get_Value() const { return _Value; }
private:
	static constexpr size_t BytesSize = sizeof(UID_t);
	union 
	{
		UID_t _Value;
		unsigned char _Bytes[BytesSize];
	};
};



CoreEnd

namespace std {

	template <>
	struct hash<UCode::UID>
	{
		std::size_t operator()(const UCode::UID& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;
			return ((hash<UCode::UID::UID_t>()(k.Get_Value())));
		}
	};

}