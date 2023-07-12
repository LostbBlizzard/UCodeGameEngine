#include "WinSeverSocket.hpp"
CoreStart
WinSeverSocket::WinSeverSocket(size_t buffersize):_IsRuning(false),_buffersize(buffersize), _MaxPlayers(0)
{
}

WinSeverSocket::~WinSeverSocket()
{
}
void WinSeverSocket::UpdateCalls()
{
}
void WinSeverSocket::MakeSever(std::string IP, SInt32 port)
{
	if (!_IsRuning)
	{
		_Clients.resize(_MaxPlayers);
		//do cool stuff
	}
}
void WinSeverSocket::ClientSocket::SendData(CallBackKeyType key, const Byte* _Bytes, size_t datasize)
{
}
CoreEnd


