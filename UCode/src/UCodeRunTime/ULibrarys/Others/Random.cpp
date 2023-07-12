#include "Random.hpp"
#include <random>
CoreStart
constexpr Random::SeedType IfSeed0 = 1 << 1;
Random::Random(SeedType seed)
{
	if (seed == 0) { seed = IfSeed0; }
	_state.Value = seed;
	
}
Random::Random(Rng_state state):_state(state)
{
}
Random::~Random()
{
}

Random Random::RandomSeed()
{	

	SeedType newseed = rand();//Random seed
	return Random(newseed);
}

/* The state word must be initialized to non-zero */
Random::SeedType Random::xorshift32(Rng_state& state)
{
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	SeedType x = state.Value;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}

CoreEnd