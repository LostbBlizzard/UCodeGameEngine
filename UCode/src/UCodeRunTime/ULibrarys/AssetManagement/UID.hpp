#pragma once
#include <xhash>
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/ULibrarys/Others/Random.hpp>
CoreStart
struct UID
{
	UID();
	using UID_t = u64;

	constexpr bool operator==(const UID& other) const { return (_Value == other._Value); }
	constexpr bool operator!=(const UID& other) const { return !operator==(other); }

	static void MakeNew(Random& rng, UID& out);


	UCodeGameEngineForceinlne void Set_Value(UID_t Value) { _Value = Value; }
	UCodeGameEngineForceinlne auto Get_Value() const { return _Value; }
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
			using std::hash;
			return ((hash<UCode::UID::UID_t>()(k.Get_Value())));
		}
	};

}