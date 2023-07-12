#include "UID.hpp"
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
		char V;
		switch (rng.GetInt<Byte>(1, 3))
		{
		case 1:V = rng.GetInt<char>('0', '9'); break;
		case 2:V = rng.GetInt<char>('A', 'Z'); break;
		case 3:V = rng.GetInt<char>('a', 'z'); break;
		default:V = '\0'; break;
		}
		out._Bytes[i] = V;
	}
}

CoreEnd

