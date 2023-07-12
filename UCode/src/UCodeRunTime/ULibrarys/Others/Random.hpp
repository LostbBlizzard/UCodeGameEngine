#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include "UCodeRunTime/UDefs.hpp"
CoreStart
class Random
{
public:
	typedef UInt32 SeedType;
	struct Rng_state {
		SeedType Value;
	};
	Random(SeedType seed);
	Random(Rng_state state);
	~Random();
	

	template<typename T> T GetInt(T min, T max)
	{
		T r = _state.Value % max + min;
		NextNumber();
		return r;
	}
	template<typename T> T GetFloat(T min, T max)
	{
		T r = min + static_cast<T>(_state.Value) / (static_cast<T>(MaxSeedValue / (max - min)));
		NextNumber();
		return r;
	}


	UCODE_ENGINE_FORCE_INLINE bool GetBool() { return GetInt<Byte>(0, 1); }



	static Random RandomSeed();
	UCODE_ENGINE_FORCE_INLINE auto Get_State() const { return _state; }
private:
	const static SeedType MaxSeedValue = UInt32MaxValue;
	
	UCODE_ENGINE_FORCE_INLINE void NextNumber()
	{
		_state.Value = xorshift32(_state);
	}
	
	static SeedType xorshift32(Rng_state& state);


	Rng_state _state;
};
CoreEnd
