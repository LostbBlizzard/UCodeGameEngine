#pragma once
#include <UCodeRunTime/Core/CoreNamespace.hpp>
#include <UCodeRunTime/BasicTypes.hpp>
#include <UCodeRunTime/UDefs.hpp>
CoreStart
class Random
{
public:
	using SeedType = u32;
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

	
	UCodeGEForceinlne bool GetBool() { return GetInt<Byte>(0, 1); }



	static Random RandomSeed();
	UCodeGEForceinlne auto Get_State() const { return _state; }
private:
	const static SeedType MaxSeedValue =UINT32_MAX;
	
	UCodeGEForceinlne void NextNumber()
	{
		_state.Value = xorshift32(_state);
	}
	
	static SeedType xorshift32(Rng_state& state);


	Rng_state _state;
};
CoreEnd
