#include "UID.hpp"
#include "../Others/Random.hpp"
CoreStart
 UID::UID()
{
	for (size_t i = 0; i < BytesSize; i++)
	{
		_Bytes[i] = 0;
	}
}
void UID::MakeNew(Random& rng, UID& out)
{
	for (size_t i = 0; i < BytesSize; i++)
	{
		out._Bytes[i] = rng.GetInt<Byte>(0,255);
	}
}

CoreEnd

